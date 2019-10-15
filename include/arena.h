//
// Created by wangxn on 2019/10/15.
//

#ifndef ICD_ARENA_H
#define ICD_ARENA_H

#include "except.h"

#define T Arena_T
typedef struct T *T;

extern const Except_T Arena_NewFailed;
extern const Except_T Arean_Failed;

//--exported functions 66--

/**
 * @return 创建一个新的内存池并返回指向新建的内存池的一个不透明指针
 * 该指针将传递给其他需要指定内存池参数的函数
 * 如果不能分配内存池,将引发Arena_NewFailed异常.
 */
extern T Arena_new(void);

/**
 * 释放与*ap内存池关联的内存,释放内存池本身,并将*ap清零
 * @param ap
 */
extern void Arena_dispose(T *ap);

/**
 * 在内存池上分配新的内存
 * @param arena   内存池
 * @param nbytes  分配的内存大小
 * @param file    定位的源文件
 * @param line    定位的文件行数
 * @return 分配的内存的指针,该内存块对齐到地址边界,能够适合于具有最严格对齐要求的数据,内存未初始化
 * @throws 无法分配内存,则已发Arena_Failed异常
 */
extern void * Arena_alloc(T arena, long nbytes, const char * file, int line);

/**
 * 在内存池上分配新的内存
 * @param arena  内存池
 * @param count  分配的子内存数量
 * @param nbytes 分配的子内存大小
 * @param file    定位的源文件
 * @param line   定位的文件行数
 * @return 分配的内存的指针,该内存块对齐到地址边界,能够适合于具有最严格对齐要求的数据,内存未初始化
 * @throws 无法分配内存,则已发Arena_Failed异常
 */
extern void *Arena_calloc(T arena, long count, long nbytes, const char * file, int line);

/**
 * 释放内存池中所有的内存
 * 相当于释放内存池中的自创建或者上一次调用Arena_free以来已分配的所有内存块
 * @param arena
 */
extern void Arean_free(T arena);

#undef T
#endif //ICD_ARENA_H
