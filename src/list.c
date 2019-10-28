//
// Created by wangxn on 2019/10/28.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#include "list.h"
#include "mem.h"
#include "assert.h"


#define T List_T

/**
 * 创建并返回一个链表,调用时,需要传递N+1个指针作为参数,前N个指针为非NULL,最后一个未NULL指针,
 * 该函数会创建一个包含N个节点的链表, 各个节点的first字段包含了N个非NULL的指针,
 * 而第N个节点的rest字段为NULL
 * @param x    这里假定可变参数的类型为 void, 作用于第二个以及后续参数的指针来说,程序员必须对char和void以外的指针显示转换.
 * @param ...
 * @return
 */
T List_list(void * x, ...) {
    va_list ap;

    T list;      //最后会返回的链表
    T *p = &list;   //链表的指针的指针

    va_start(ap, x);    //遍历可变参数列表
    for(; x; x=va_arg(ap, void *)) {
        NEW(*p);         //如果是第一个结点,分配新的空间,将地址传递给*p,
        (*p)->first = x;    //给结点赋值
        p = &(*p)->rest;    //获取指向下一个结点的地址, 并传递给p,开始下一个循环
    }

    *p = NULL;

    va_end(ap);
    return list;
}


/**
 * 将一个链表附加到另一个上,该函数将tail赋值给list中最后一个结点的rest字段,
 * 如果list 为NULL, 则返回tail
 * @param list
 * @param tail
 * @return
 */
T List_append(T list, T tail) {
    T * p = & list;
    while(*p) {
        p = &(*p)->rest;  //遍历直到链表尾部
    }
    *p = tail;
    return list;
}

/**
 * 赋值链表, 返回一个新的链表
 * @param list
 * @return
 */
T List_copy(T list) {
    T head;
    T * p = &head;

    for(; list; list = list->rest) {
        NEW(*p);
        (*p)->first = list->first;
        p = &(*p)->rest;
    }
    *p = NULL;
    return head;
}


/**
 * 给定一个非空的链表, 将第一个结点的first字段赋值给 *x (如果x不是NULL指针),
 * 移除第一个结点并且释放其内存,最后返回结构链表.给定一个空链表,List_pop只是返回原链表,并不修改*x
 * @param list
 * @param x
 * @return
 */
T List_pop(T list, void ** x) {
    if(list) {
        T head = list->rest;
        if(x) {
            *x = list->first;
        }
        FREE(list);
        return head;
    } else {
        return list;
    }
}

/**
 * List_push在链表的起始处添加一个包含x的新结点,并返回新的链表.
 * List_push可能引发Mem_Failed异常
 * @param list  可以为NULL, 为NULL时,将创建一个新的链表
 * @param x  添加到链表中的元素
 * @return
 */
T List_push(T list, void * x) {
    T p;  //分配一个结点
    NEW(p);   //初始化这个结点
    p->first = x;   //为结点赋值
    p->rest = list;
    return p;   //返回结点
}

/**
 * 逆转链表的结点顺序,而后返回新的结果链表
 * @param list
 * @return
 */
T List_reverse(T list) {
    T head = NULL;
    T next;

    for(; list; list = next) {
        next = list->rest;
        list->rest =head;
        head = list;
    }
    return head;
}


/**
 * @param list
 * @return 返回链表中的结点数目
 */
T List_length(T list) {
    int n;
    for (n = 0;  list ; list = list->rest) {
        n++;
    }
    return n;
}

/**
 * 如果*list不为NULL, List_free将释放*list链表中的所有结点并将其设置为NULL指针.
 * 如果*list为NULL,则不处理
 * @param list  链表指针的指针
 */
void List_free(T * list) {
    T next;
    assert(list);
    for(; *list; *list = next) {
        next = (*list)->rest;
        FREE(*list);
    }
}

/**
 * 遍历list链表中的每一个结点,并且对结点调用apply指向的函数,并接受额外的一个参数 *cl
 * @param list
 * @param apply   函数指针
 * @param cl
 */
void List_map(T list, void apply(void ** x, void * cl), void * cl) {
    assert(apply);
    for(; list; list=list->rest) {
        apply(&list->first, cl);
    }
}

/**
 * 将包含N个值的链表转换成一个数组,数组中的元素0到N-1分别包含链表中N个结点的first字段值,
 * 数组中的元素N包含end值,end通常是一个NULL指针.
 * 可能引发Mem_Failed异常
 * @param list
 * @param end
 * @return 返回一个指向数组第一个元素的指针
 */
void ** List_toArray(T list, void * end) {
    int i;
    int n = List_length(list);

    void ** array = ALLOC((n + 1) * sizeof(*array));

    for(i = 0; i< n; i++) {
        array[i] = list->first;
        list = list->rest;
    }
    array[i] = end;
    return array;
}