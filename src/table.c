//
// Created by wangxn on 2019/10/29.
//

#include "table.h"
#include "mem.h"
#include "assert.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T Table_T

struct T {
    int size;                                    //表的大小,分配的桶的大小
    int (*cmp)(const void * x, const void * y);  //元素比较函数
    unsigned (*hash)(const void * key);          //元素的hash函数
    int length;                                  //表中binding实例的数目
    long timestamp;                              // Table_put或Table_remove每次修改表时, 表的timestamp都会+1

    struct  binding {                           //元素的键值对封装, 元素的链表封装, 同一个链表中的元素具有相同的额hash值
        struct binding * link;
        const void * key;
        void * value;
    } ** buckets;
};

/**
 * 默认的比较函数
 * @param x
 * @param y
 * @return
 */
static int cmpatom(const void * x, const void * y) {
    return x != y;
}

/**
 * 默认的hash函数
 * 原子是一个地址, 这个地址本省就可以用来作为hash码,
 * 右移两位是因为可能每个原子都起始于字边界(word boundary),因此最右侧两位可能是0
 * @param key
 * @return
 */
static unsigned  hashatom(const void * key) {
    return (unsigned long) key>>2;
}


/**
 * 可能引发Mem_Failed异常
 * @param hint  用于估计新的表中预期会容纳的表项数量,无论hint值如何,所有表都可以容纳任意数目的表项
 * @param cmp   操作键值  比较函数
 * @param hash  操作键值  hash函数
 * @return
 */
T Table_new(int hint, int cmp(const void * x, const void * y), unsigned hash(const void * key)) {

    T table;
    int i;
    // primes中的值接近于2的n次方的素数(n=9...16),
    static int primes[] = {
            509, 509, 1021, 2053, 4093,
            9191, 16381, 32771, 65521, INT_MAX
    };

    assert(hint >= 0);

    for(i =1; primes[i] < hint; i++);
    //分配空间
    table = ALLOC(sizeof(*table) + primes[i - 1] * sizeof(table->buckets[0]));
    //设置属性
    table->size = primes[i - 1];
    table->cmp = cmp ? cmp : cmpatom;
    table->hash = hash ? hash : hashatom;

    table->buckets = (struct binding **)(table + 1);
    for(i = 0; i< table->size; i++) {
        table->buckets[i] = NULL;
    }
    table->length = 0;
    table->timestamp = 0;
    return table;
}

/**
 * 释放table, 并将其设置为NULL,
 * Table_free并不释放键或者值
 * Table_free必须释放各个binding结构实例和Table_T结构本身.
 * @param table
 */
void Table_free(T * table) {
    assert(table && *table);

    if ((*table)->length > 0) {
        int i;

        struct  binding *p, *q;
        for (i = 0;  i < (*table)->size ; i++) {
            for( p = (*table)->buckets[i]; p; p = q) {
                q = p->link;
                FREE(p);
            }
        }
    }

    FREE(*table);
}

/**
 *
 * @param table
 * @return 返回表中键的数目
 */
int Table_length(T table) {
    assert(table);
    return table->length;
}

/**
 * 添加一个新的键值对
 * 它在表中查找一个键,如果找到了,则改变相关的值,
 * 如果没有找到,则新分配并初始化一个新的binding结构实例,并添加到链表的头部.
 * 添加到头部是最容易且最高效的方案
 * @param table
 * @param key
 * @param value
 * @return
 */
void * Table_put(T table, const void *key, void * value) {
    int i;
    struct binding * p;
    void * prev;

    assert(table);
    assert(key);

    //--search table for key 94--
    i = (*table->hash)(key) % table->size;  //通过hash函数计算得到链表桶的索引位置

    for(p = table->buckets[i]; p; p = p->link) {  //遍历链表
        if((*table->cmp)(key, p->key) == 0) {    //通过比较函数cmp来确定找到相应的键值对
            break;
        }
    }

    if (p == NULL) {        //没有找到
        NEW(p);
        p->key = key;
        p->link = table->buckets[i];
        table->buckets[i] = p;
        table->length++;
        prev = NULL;
    } else {                //找到了
        prev = p->value;
    }

    p->value = value;
    table->timestamp ++;
    return prev;
}

/**
 * 获取Table中对应key 的值
 * @param table
 * @param key
 * @return
 */
void * Table_get(T table, const void *key) {
    int i;
    struct binding * p;
    assert(table);
    assert(key);

    //--search table for key 94--
    i = (*table->hash)(key) % table->size;  //通过hash函数计算得到链表桶的索引位置

    for(p = table->buckets[i]; p; p = p->link) {  //遍历链表
        if((*table->cmp)(key, p->key) == 0) {    //通过比较函数cmp来确定找到相应的键值对
            break;
        }
    }

    return p ? p->value : NULL;
}

/**
 * 移除Table中某一个键值对
 * @param talbe
 * @param key
 * @return
 */
void * Table_remove(T table, const void * key) {
    assert(table);
    assert(key);

    int i;
    struct binding **pp;

    table->timestamp++;
    i = (*table->hash)(key) % table->size;  //根据hash函数找到索引的位置

    for(pp = &table->buckets[i]; *pp; pp = &(*pp)->link) {  //遍历桶中的元素
        if ((*table->cmp)(key, (*pp)->key) == 0) {  //根据cmp比较函数来最终找到元素
            struct binding * p = *pp; //保留一个副本
            void *value = p->value;   //得到需要返回的结果的实际值
            *pp = p->link;            //这里是指针赋值, 直接替换指针值
            FREE(p);                  //释放空间
            table->length--;          //元素数目减1
            return value;
        }
    }
    return NULL;

}


/**
 * 遍历Table, 针对每一个键值对,调用apply函数,
 * @param table
 * @param apply
 * @param cl
 */
void Table_map(T table, void apply(const void *key, void ** value, void * cl), void * cl) {
    assert(table);
    assert(apply);

    int i;
    unsigned stamp;
    struct binding *p;

    stamp = table->timestamp;
    for (i = 0; i < table->size ; i++) {
        for (p = table->buckets[i];  p ; p = p->link) {
            apply(p->key, &p->value, cl);
            assert(table->timestamp == stamp);  //这里防止在遍历时,有其他地方在修改表中的内容
        }
    }
}

/**
 * 遍历Table,将键和值全部放入到数组中返回
 * 包含N个键值对的Table,本函数会构建一个有2N+1个元素的数组,并返回第一个元素的指针,
 * 在数组中键和值会交替出现,
 * 可能会引发Mem_failed异常
 * @param table
 * @param end
 * @return
 */
void ** Table_toArray(T table, void * end) {
    assert(table);

    int i;
    int j = 0;

    void ** array;
    struct binding *p;

    //分配数组
    array = ALLOC((2 * table->length + 1) * sizeof(*array));

    for( i = 0; i < table->size; i++) {  //遍历桶
        for( p = table->buckets[i]; p; p = p->link) {  //遍历桶中的每个元素
            array[j++] = (void *)p->key;
            array[j++] = p->value;
        }
    }
    array[j] = end;
    return array;
}

