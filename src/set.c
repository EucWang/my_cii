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
    unsigned timestamp;  //用于实现检查错误,防止多线程情况下,同时修改集合数据
    int size;

    int (*cmp)(const void *x, const void *y);
    unsigned (*hash)(const void *x);

    struct member{
        struct member * link;
        const void *member;
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

    set = ALLOC(sizeof(*set) + primes[i-1] * sizeof(set->buckets[0]));

    set->size = primes[i -1];
    set->cmp = cmp?cmp:cmpatom;
    set->hash = hash?hash:hashatom;
    set->buckets = (struct member **)(set + 1);

    for (i = 0; i < set->size ; i++) {
        set->buckets[i] = NULL;
    }

    set->length = 0;
    set->timestamp = 0;
    return set;
}

/**
 * 释放set集合
 * @param set
 */
void Set_free(T *set);

/**
 * 返回集合包含的成员的数目
 * @param set
 * @return
 */
int Set_length(T set);

/**
 * 判断member是否在集合中
 * @param set
 * @param member
 * @return  在集合中返回1, 否则返回0
 */
int Set_member(T set, const void *member);

/**
 * 将member添加到set集合中
 * 可能引发Mem_Failed异常
 * @param set
 * @param member
 */
void Set_put(T set, const void *member);

/**
 * 将member从Set集合中移除
 * 并返回被删除的成员
 * 如果member不在集合set中,则什么也不做
 * @param set
 * @param member
 * @return
 */
void *Set_remove(T set, const void *member);

/**
 * 对集合中的每一个成员都调用apply函数
 * @param set
 * @param apply
 * @param cl
 */
void Set_map(T set, void apply(const void *member, void *cl), void *cl);

/**
 * 将集合转换成数组,
 * 返回一个指针, 指向一个N+1个元素的数组,
 * 其中任意顺序包含了集合的N个元素,
 * 可能引发Mem_Failed异常
 * @param set
 * @param end  end的值赋值给数组的第N+1个元素. 一般为NULL
 * @return
 */
void **Set_toArray(T set, void *end);

//----------------------------------------------------------------------

/**
 * 返回集合s和t的并集  s+t
 * 并集是包含s和t所有成员的一个集合
 * @param s
 * @param t
 * @return
 */
T Set_union(T s, T t);

/**
 * 返回集合的交集 s*t
 * 交集包含所有即出现在s中,也出现在t中的成员
 * @param s
 * @param t
 * @return
 */
T Set_inter(T s, T t);

/**
 * 返回集合的差集 s-t
 * 差集包含所有仅出现在集合s中,而不出现在集合t中的成员
 * @param s
 * @param t
 * @return
 */
T Set_minus(T s, T t);

/**
 * 返回集合的 对称差  s/t
 * 对称差包含了所有仅出现在集合s或者集合t其中之一的成员
 * @param s
 * @param t
 * @return
 */
T Set_diff(T s, T t);