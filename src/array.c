//
// Created by wangxn on 2019/11/25.
//

#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"

#define T Array_T

/**
 * 初始化array指向的Array_T结构实例的各个字段,将其分别设置为参数length,size he ary的值
 *
 *
 * @param array
 * @param length
 * @param size
 * @param ary
 */
extern void ArrayRep_init(T array, int length, int size, void * ary) {
    assert(array);
    assert(ary && length > 0 || length == 0 && ary == NULL);

    assert(size > 0);
    array->length = length;
    array->size = size;

    if(length > 0) {
        array->array = ary;
    } else {
        array->array = NULL;
    }
}

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
extern T Array_new(int length, int size){
    T array;

    NEW(array);

    if(length > 0) {
        ArrayRep_init(array, length, size, CALLOC(length, size));
    } else {
        ArrayRep_init(array, length, size, NULL);
    }

    return array;
}



/**
 * 释放*array并将其清零,
 * Array_free无需检查(*array)->array是否为NULL, 因为FREE可以处理NULL指针
 * @param array
 */
extern void Array_free(T * array) {
    assert(array && *array);

    FREE((*array)->array);
    FREE(*array);
}

/**
 * 返回元素的数目
 * @param array
 * @return
 */
extern int Array_length(T array) {
    assert(array);
    return array->length;
}

/**
 * 返回数组中单个元素的字节数
 * @param array
 * @return
 */
extern int Array_size(T array) {
    assert(array);
    return array->size;
}

/**
 * 返回下标为i的元素的指针
 * @param array
 * @param i   i必须是>=0 且 < Array_length(),否则会抛出异常
 * @return
 */
extern void * Array_get(T array, int i) {
    assert(array);
    assert(i >= 0  && i < array->length);
    return array->array + i * array->size;
}

/**
 * 用elem指向的新元素,覆盖元素i的值,
 *
 * @param array           数组
 * @param i             i必须是>=0 且 < Array_length(),否则会抛出异常
 * @param elem         元素的指针
 * @return  返回elem的值
 */
extern void * Array_put(T array, int i, void * elem) {
    assert(array);
    assert(i >= 0 && i < array->length);
    assert(elem);

    memcpy(array->array + i * array->size, elem, array->size);
    return elem;
}

/**
 *改变array的大小,使得其能够容纳length个元素, 会将原来的内存地址数据复制到新的内存块中
 * 会根据需要扩展或者收缩数组
 * 可能引发Mem_Failed异常
 * @param array
 * @param length  必须大于等于0
 */
extern void Array_resize(T array, int length) {
    assert(array);
    assert(length >= 0);
    if(length == 0) {
        FREE(array->array);
    } else if(array->length == 0) {  //原始的数组长度就为0
        array->array = ALLOC(length * array->size);  //直接分配内存地址
    } else {
        RESIZE(array->array, length * array->size);  //从新分配地址
    }
    array->length = length;
}

/**
 * 返回array的一个副本,包含array的前length个元素
 * 如果length超过array中元素的数目,副本中过多的那些元素将被初始化为0
 * 可能引发Mem_Failed异常
 * @param array
 * @param length
 * @return
 */
extern T Array_copy(T array, int length) {
    T copy;
    assert(array);
    assert(length >= 0);
    copy = Array_new(length, array->size);
    if(copy->length >= array->length && array->length > 0) {  //副本的长度大于原始数组的长度, 直接复制过去就行了
        memcpy(copy->array, array->array, array->length * array->size);
    } else if (array->length > copy->length && copy->length > 0) {    //副本的长度小于原始数组的长度, 只复制副本长度的部分
        memcpy(copy->array, array->array, copy->length * array->size);
    }

    return copy;
}


/**
 * 将array中元素的数目和每个元素的大小分别该成length和size
 * @param array
 * @param length
 * @param size
 */
extern void Array_reshape(T array, int length, int size) {
    assert(array);
    assert(length >= 0 && size >= 0);
    if (length == 0) {

        FREE(array->array);       //释放内存地址
    } else if (array->length == 0) {  //原始的数组长度就为0

        array->array = ALLOC(length * size);  //直接分配内存地址
    } else {

        if (size == array->size) {
            Array_resize(array, length);
        } else if (size > array->size) {
            //size大于原本各个元素的大小, 超出的部分字节设置为0

            void *pVoid = CALLOC(length , size);

            for (int i = 0; i < length; ++i) {
                if (i >= array->length) break;
                void *get = Array_get(array, i);
                void * item = (pVoid + i * size);
                for (int j = 0; j < array->size; ++j) {
                    *(char *)(item + j) = *(char *)(get + j);
                }
            }
            array->array = pVoid;
        } else if (size < array->size) {
            //size小于原本的每个元素的大小,  则截断原来的各个元素,
            void *pVoid = ALLOC(length * size);
            for (int i = 0; i < length; ++i) {
                void *get = Array_get(array, i);
                void * item = (pVoid + i * size);
                for (int j = 0; j < size; ++j) {
                    *(char *)(item + j) = *(char *)(get + j);
                }
            }
            array->array = pVoid;
        }
    }

    array->size = size;
    array->length = length;

}