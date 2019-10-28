//
// Created by wangxn on 2019/10/28.
//

#ifndef ICD_LIST_H
#define ICD_LIST_H

#define T List_T
typedef struct T * T;

/**
 * 大部分ADT都隐藏其类型的表示细节.
 * 链表展现了这些细节
 */
struct T {
    T rest;         // 下一个元素节点的指针
    void * first;   // 链表中存储的值
};

/**
 * 将一个链表附加到另一个上,该函数将tail赋值给list中最后一个结点的rest字段,
 * 如果list 为NULL, 则返回tail
 * @param list
 * @param tail
 * @return
 */
extern T List_append(T list, T tail);

/**
 * 创建并返回一个链表,调用时,需要传递N+1个指针作为参数,前N个指针为非NULL,最后一个未NULL指针,
 * 该函数会创建一个包含N个节点的链表, 各个节点的first字段包含了N个非NULL的指针,
 * 而第N个节点的rest字段为NULL
 * @param x    这里假定可变参数的类型为 void, 作用于第二个以及后续参数的指针来说,程序员必须对char和void以外的指针显示转换.
 * @param ...
 * @return
 */
extern T List_list(void * x, ...);

/**
 * 给定一个非空的链表, 将第一个结点的first字段赋值给 *x (如果x不是NULL指针),
 * 移除第一个结点并且释放其内存,最后返回结构链表.给定一个空链表,List_pop只是返回原链表,并不修改*x
 * @param list
 * @param x
 * @return
 */
extern T List_pop(T list, void ** x);

/**
 * List_push在链表的起始处添加一个包含x的新结点,并返回新的链表.
 * List_push可能引发Mem_Failed异常
 * @param list  可以为NULL, 为NULL时,将创建一个新的链表
 * @param x  添加到链表中的元素
 * @return
 */
extern T List_push(T list, void * x);

/**
 * 逆转链表的结点顺序,而后返回新的结果链表
 * @param list
 * @return
 */
extern T List_reverse(T list);

/**
 * 赋值链表, 返回一个新的链表
 * @param list
 * @return
 */
extern T List_copy(T list);

/**
 * @param list
 * @return 返回链表中的结点数目
 */
extern T List_length(T list);

/**
 * 如果*list不为NULL, List_free将释放*list链表中的所有结点并将其设置为NULL指针.
 * 如果*list为NULL,则不处理
 * @param list  链表指针的指针
 */
extern void List_free(T * list);

/**
 * 遍历list链表中的每一个结点,并且对结点调用apply指向的函数,并接受额外的一个参数 *cl
 * @param list
 * @param apply   函数指针
 * @param cl
 */
extern void List_map(T list, void apply(void ** x, void * cl), void * cl);

/**
 * 将包含N个值的链表转换成一个数组,数组中的元素0到N-1分别包含链表中N个结点的first字段值,
 * 数组中的元素N包含end值,end通常是一个NULL指针.
 * 可能引发Mem_Failed异常
 * @param list
 * @param end
 * @return 返回一个指向数组第一个元素的指针
 */
extern void ** List_toArray(T list, void * end);

#undef T
#endif //ICD_LIST_H
