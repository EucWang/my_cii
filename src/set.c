//
// Created by wangxn on 2019/10/31.
//
#include "set.h"
#include "mem.h"
#include "assert.h"
#include "atom.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#define T Set_T

struct T {
    int length;          //集合中成员的数目
    unsigned int timestamp;  //用于实现检查错误,防止多线程情况下,同时修改集合数据
    int size;

    int (*cmp)(const void *x, const void *y);
    unsigned (*hash)(const void *x);

    struct Member{
        struct Member * link;
        const void * member;
    } **buckets;
};

/**
 * 创建set集合, 初始化并返回一个Set实例
 * @param hint
 * @param cmp
 * @param hash
 * @return
 */
T Set_new(int hint,
                 int cmp(const void *x, const void *y),
                 unsigned hash(const void *x)) {
    T set;
    int i;
    static int primes[] = {
            509, 509, 1021,2053, 4093, 8191, 16381, 32771, 65521, INT_MAX
    };

    assert(hint>=0);

    for (i = 0; primes[i] < hint; i++) ;

    if (i == 0) i = 1;

    set = ALLOC(sizeof(*set) + primes[i-1] * sizeof(set->buckets[0]));

    set->size = primes[i -1];
    set->cmp = cmp?cmp:cmpatom;
    set->hash = hash?hash:hashatom;
    set->buckets = (struct Member **)(set + 1);

    for (i = 0; i < set->size ; i++) {
        set->buckets[i] = NULL;
    }

    set->length = 0;
    set->timestamp = 0;
    return set;
}

/**
 * 释放set集合

 * 如果集合为非空, Set_free首先必须遍历各个哈希链,释放其中的member结构实例, 
 * 然后才能释放集合本身并将*set清零
 * @param set
 */
void Set_free(T *set) {
    assert(set && *set);

    if((*set)->length > 0) {
        int i;
        struct Member * p;
        struct Member * q;
        for(i =0; i < (*set)->size; i++) {
            for(p = (*set)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
        }
    }
    FREE(*set);
}

/**
 * 返回集合包含的成员的数目
 * @param set
 * @return
 */
int Set_length(T set) {
    assert(set);
    return set->length;
}

/**
 * 判断member是否在集合中
 * @param set
 * @param member
 * @return  在集合中返回1, 否则返回0
 */
int Set_member(T set, const void * member) {
    int i;
    struct Member * p;

    assert(set);
    assert(member);

    //--- search set fro member 109---- 如果搜索取得成功, 则p不是NULL,否则为NULL
    i = (*set->hash)(member) % set->size;
    for(p = set->buckets[i]; p; p = p->link) {
        if((*set->cmp)(member, p->member) == 0){
            break;
        }
    }

    return p != NULL;
}

/**
 * 将member添加到set集合中
 * 可能引发Mem_Failed异常
 * @param set
 * @param member
 */
void Set_put(T set, const void *member) {
    int i;
    struct Member * p;

    assert(set);
    assert(member);

    //---search set for member 109---
    i = (*set->hash)(member) % set->size;
    for(p = set->buckets[i]; p; p = p->link) {
        if((*set->cmp)(member, p->member) == 0){
            break;
        }
    }

    if(p == NULL) {
        //---add member to set 109-----
        NEW(p);
        p->member = member;
        p->link = set->buckets[i];
        set->buckets[i] = p;
        set->length++;

    } else {
        p->member = member;
    }

    set->timestamp++;  //timestamp用于在Set_map中,以便强制实施已检查的运行时错误
}

/**
 * 将member从Set集合中移除
 * 并返回被删除的成员
 * 如果member不在集合set中,则什么也不做

 * 使用一个指向member结构的双重指针pp遍历适当的哈希链,直到 *pp == NULL 或者 (*pp)->member是我们需要的结果
 * 得到结果时, 就可以从哈希链中删除该成员了
 * @param set
 * @param member
 * @return
 */
void *Set_remove(T set, const void *member) {

    int i;
    struct Member ** pp;

    assert(set);
    assert(member);
    set->timestamp++;

    i = (*set->hash)(member) % set->size;

    for(pp= &(set->buckets[i]); *pp; pp = &(*pp)->link) {
        if ((*set->cmp)(member, (*pp)->member) == 0) {  //相等的情况
            struct Member * p = *pp;
            *pp = p->link;
            member = p->member;
            FREE(p);
            set->length--;
            return (void *)member;
        }
    }

    return NULL;
}

/**
 * 对集合中的每一个成员都调用apply函数
 * @param set
 * @param apply
 * @param cl
 */
void Set_map(T set, void apply(const void *member, void *cl), void *cl) {
    int i;
    unsigned stamp;
    struct Member * p;

    assert(set);
    assert(apply);
    stamp = set->timestamp;

    for(i = 0; i < set->size; i++) {
        for(p = set->buckets[i]; p; p = p->link) {
            apply(p->member, cl);   //这里是将每个成员传递给apply函数,而不是成员的指针, 所以这里不能改变成员的值
            assert(set->timestamp == stamp);
        }
    }
}

/**
 * 将集合转换成数组,
 * 返回一个指针, 指向一个N+1个元素的数组,
 * 其中任意顺序包含了集合的N个元素,
 * 可能引发Mem_Failed异常
 * @param set
 * @param end  end的值赋值给数组的第N+1个元素. 一般为NULL
 * @return
 */
void **Set_toArray(T set, void *end) {
    int i;
    int j = 0;
    void ** array;
    
    struct Member * p;
    assert(set);
    array = ALLOC((set->length + 1) * sizeof(*array));
    for(i = 0; i < set->size; i++) {
        for(p = set->buckets[i]; p; p = p->link) {
            array[j++] = (void *)p->member;
        }
    }
    array[j] = end;
    return array;
}

//----------------------------------------------------------------------

/**
 *
 * @param t
 * @param hint
 * @return
 */
static T copy(T t, int hint) {
    T set;
    assert(t);

    set = Set_new(hint, t->cmp, t->hash);

    // for each member q in t 112
    int i;
    struct Member * q;
    for(i = 0; i < t->size; i++) {
        for ( q = t->buckets[i]; q; q = q->link) {
            // add q->member to set 112
            struct Member * p;
            const void * member = q->member;
            int j = (*set->hash)(member) % set->size;

            //add member to set 109
            NEW(p);
            p->member = member;
            p->link = set->buckets[j];
            set->buckets[j] = p;
            set->length++;
        }
    }

    return set;
}

/*返回两个数中较大的一个*/
static int Arith_max(int a, int b) {
    if (a >= b) {
         return a;
    } else {
        return b;
    }
}

/*返回2个数中较小的一个*/
static int Arith_min(int a, int b) {
    if(a >= b) {
         return b;
    } else {
        return a;
    }
}

/**
 * 返回集合s和t的并集  s+t
 * 并集是包含s和t所有成员的一个集合
 * @param s Set_T
 * @param t Set_T
 * @return  Set_T
 */
T Set_union(T s, T t) {
    if (s == NULL) {
        assert(t);
        return copy(t, t->size);
    } else if(t == NULL) {
        return copy(s, s->size);
    } else {
        T set = copy(s, Arith_max(s->size, t->size));  //把集合s复制一份

        //两个集合的比较函数和hash函数必须相同
        assert(s->cmp == t->cmp && s->hash == t->hash);


        // for each member q in t 112
        int i;
        struct Member * q;
        for(i = 0; i < t->size; i++) {
            for ( q = t->buckets[i]; q; q = q->link) {
                Set_put(set, q->member);  //将集合t的元素添加到复制的s集合中
            }
        }
        return set;
    }
}

/**
 * 返回集合的交集 s*t
 * 交集包含所有即出现在s中,也出现在t中的成员
 * @param s Set_T
 * @param t Set_T
 * @return  Set_T
 */
T Set_inter(T s, T t) {

    if (s == NULL) {
        assert(t);
        return Set_new(t->size, t->cmp, t->hash); //只要有一个集合为空, 则返回一个空的集合
    } else if( t == NULL) {
        return Set_new(s->size, s->cmp, s->hash); //只要有一个集合为空, 则返回一个空的集合
    } else if(s->length < t->length) {   //交换2个集合的位置, 保证第一个集合的元素数量大于第二个集合
        return Set_inter(t, s);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);

        assert(s->cmp == t->cmp && s->hash == t->hash);

        // for each member q in t 112
        int i;
        struct Member * q;
        for(i = 0; i < t->size; i++) {
            for ( q = t->buckets[i]; q; q = q->link) {
                if(Set_member(s, q->member)) {

                    // add q->member to set 112
                    struct Member * p;
                    const void * member = q->member;
                    int j = (*set->hash)(member) % set->size;

                    //add member to set 109
                    NEW(p);
                    p->member = member;
                    p->link = set->buckets[j];
                    set->buckets[j] = p;
                    set->length++;

                }
            }
        }
        return set;
    }

}

/**
 * 返回集合的差集 t - s
 * 差集包含所有仅出现在集合t中,而不出现在集合s中的成员
 * @param t  注意, 这里集合t是第一个参数
 * @param s
 * @return
 */
T Set_minus(T t, T s) {

    if (t == NULL) {
        assert(s);
        return Set_new(s->size, s->cmp, s->hash);  //集合t为空,则返回一个空集合
    } else if(s == NULL) {         //集合s为空,则返回集合t的复制集合
        return copy(t, t->size);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);  //创建一个新的集合

        assert(s->cmp == t->cmp && s->hash == t->hash);

        // for each member q in t 112
        int i;
        struct Member * q;
        for(i = 0; i < t->size; i++) {
            for ( q = t->buckets[i]; q; q = q->link) {
                if(!Set_member(s, q->member)) {  //t中的元素不在集合s中, 则将该元素复制一份,放入到最终的结果中

                    // add q->member to set 112
                    struct Member * p;
                    const void * member = q->member;
                    int j = (*set->hash)(member) % set->size;

                    //add member to set 109
                    NEW(p);
                    p->member = member;
                    p->link = set->buckets[j];
                    set->buckets[j] = p;
                    set->length++;
                }
            }
        }
        return set;
    }
}

/**
 * 返回集合的 对称差  s/t
 * 对称差包含了所有仅出现在集合s或者集合t其中之一的成员
 * @param s
 * @param t
 * @return
 */
T Set_diff(T s, T t) {

    if(s == NULL) {
        assert(t);
        return copy(t, t->size);
    } else if(t == NULL) {
        return copy(s, s->size);
    } else {
        T set = Set_new(Arith_min(s->size, t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);

        //--for each member q in t 112--
        int i;
        struct Member * q;
        for(i = 0; i < t->size; i++) {
            for ( q = t->buckets[i]; q; q = q->link) {
                if(!Set_member(s, q->member)) {  //t中的元素不在集合s中, 则将该元素复制一份,放入到最终的结果中

                    // add q->member to set 112
                    struct Member * p;
                    const void * member = q->member;
                    int j = (*set->hash)(member) % set->size;

                    //add member to set 109
                    NEW(p);
                    p->member = member;
                    p->link = set->buckets[j];
                    set->buckets[j] = p;
                    set->length++;
                }
            }
        }

        //交互2个集合的位置,然后把上面的遍历再来一次,就可以把2个集合中不相同的元素都放入到新的集合中了
        T u = t;
        t = s;
        s = u;

        for(i = 0; i < t->size; i++) {
            for ( q = t->buckets[i]; q; q = q->link) {
                if(!Set_member(s, q->member)) {  //t中的元素不在集合s中, 则将该元素复制一份,放入到最终的结果中

                    // add q->member to set 112
                    struct Member * p;
                    const void * member = q->member;
                    int j = (*set->hash)(member) % set->size;

                    //add member to set 109
                    NEW(p);
                    p->member = member;
                    p->link = set->buckets[j];
                    set->buckets[j] = p;
                    set->length++;
                }
            }
        }

        return set;
    }

}