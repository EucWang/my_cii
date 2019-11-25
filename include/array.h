//
// Created by wangxn on 2019/11/25.
//

#ifndef ICD_ARRAY_H
#define ICD_ARRAY_H


#define T Array_T
typedef struct T * T;

//exported functions 117

/**
 * 分配,初始化一个新的数组, 包含length个元素, 可以用索引 0 ~ length-1 的下标来访问
 * 如果length=0, 则数组不包含任何元素
 * 每个元素占size个字节,每个元素各个字节都初始化为0, size必须包含对齐所需的填充字节
 * 如果length为负值或者size为负值,则抛出错误,
 * Array_new可能引发Mem_Fialed异常
 *
 * Array接口对于元素的值不做任何限制,每个元素只是一个字节序列,包含size个字节.
 * 这样的设计, Array_T通常用于构建其他ADT
 *
 * @param length  数组元素的个数
 * @param size    数组单个元素的字节数
 * @return    Array_T结构
 */
extern T Array_new(int length, int size);

/**
 * 释放*array并将其清零,
 * @param array
 */
extern void Array_free(T * array);

/**
 * 返回元素的数目
 * @param array
 * @return
 */
extern int Array_length(T array);

/**
 * 返回数组中单个元素的字节数
 * @param array
 * @return
 */
extern int Array_size(T array);

/**
 * 返回下标为i的元素的指针
 * @param array
 * @param i   i必须是>=0 且 < Array_length(),否则会抛出异常
 * @return
 */
extern void * Array_get(T array, int i);

/**
 * 用elem指向的新元素,覆盖元素i的值,
 *
 * @param array           数组
 * @param i             i必须是>=0 且 < Array_length(),否则会抛出异常
 * @param elem         元素的指针
 * @return  返回elem的值
 */
extern void * Array_put(T array, int i, void * elem);

/**
 *改变array的大小,使得其能够容纳length个元素
 * 会根据需要扩展或者收缩数组
 * 可能引发Mem_Failed异常
 * @param array
 * @param length
 */
extern void Array_resize(T array, int length);

/**
 * 返回array的一个副本,包含array的前length个元素
 * 如果length超过array中元素的数目,副本中过多的那些元素将被初始化为0
 * 可能引发Mem_Failed异常
 * @param array
 * @param length
 * @return
 */
extern T Array_copy(T array, int length);

/**
 * 将array中元素的数目和每个元素的大小分别该成length和size
 * @param array
 * @param length
 * @param size
 */
extern void Array_reshape(T array, int length, int size);


#undef T

#endif //ICD_ARRAY_H
