#ifndef MEM_H_
#define MEM_H_

#include "except.h"

//--------------------- exceptions 51  ---------------------
extern const Except_T Mem_Failed;

//--------------------- functions 51   ---------------------
/**
* 分配一块至少为nbytes长度的内存,并返回指向其中第一个字节的指针.
* 该块的内容未初始化
* 如果nbytes不是正数,会抛出异常
*
* 如果不能分配所需内存,则引发Mem_Failed异常
**/
extern void * Mem_alloc(long nbytes, const char * file, int line);

/**
* 分配一个足够大的内存块,可以容纳一个包含count个元素的数组,
* 每个数组项的长度为nbytes,并返回指向第一个数组元素的指针.
* 如果count,nbytes不是正数,会抛出异常
* 如果不能分配所需内存,则引发Mem_Failed异常
**/
extern void * Mem_calloc(long count, long nbytes, const char * file, int line);

/**
 * 内存释放
 **/
extern void Mem_free(void *ptr, const char * file, int line);

/**
* 修改 Mem_alloc, Mem_calloc, Mem_resize 分配的内存块的长度
* 可以扩展或者缩减该内存块,
* Mem_resize为改变块的长度可能会移动其位置,
* 在逻辑上等价于分配一个新的块,将ptr指向的一部分或者全部数据复制到新的内存块,并释放ptr指向的内存块
* 如果nbytes大于ptr原有的内存块长度,那么超出部分的字节是未初始化的.否则,ptr开头的nbytes字节会复制到新的内存块
* 如果不能分配新的内存,则引发Mem_Failed异常.
**/
extern void *Mem_resize(void *ptr, long nbytes, const char * file, int line);

/**
* 对每个已分配的内存块调用ptr_apply指向的函数, 
*   ptr_apply第一个参数ptr 是内存块的地址
*   第二个参数 size是块的分配长度
*   第三个,第四个参数 file 和line 是其分配的文件和坐标
* 
**/
// extern void Mem_leak(apply(void * ptr, long size, const char * file, int line, void cl), void *cl);
extern void Mem_leak(void * ptr_apply, void *cl);


//--------------------- macros 51  ---------------------

/**
* 如果不能分配所需内存,则引发Mem_Failed异常
**/
#define ALLOC(nbytes) Mem_alloc((nbytes), __FILE__, __LINE__)

/**
* 如果不能分配所需内存,则引发Mem_Failed异常
**/
#define CALLOC(count, nbytes) Mem_calloc((count), (nbytes), __FILE__, __LINE__)

/**
* 用于指针的内存分配的宏
**/
#define NEW(p) ((p) = ALLOC((long)sizeof *(p)))

/**
* 用于指针的内存分配的宏,并且清空内存
**/
#define NEW0(p) ((p) = CALLOC(1, (long)sizeof * (p)))

/**
 * 释放内存
 * 如果ptr为NULL, Mem_free没有效果
 * 如果ptr不为NULL, Mem_free将释放内存,并将ptr设置为NULL
 * 
 **/
#define FREE(ptr) ((void)(Mem_free((ptr), __FILE__, __LINE__), (ptr) = 0))

/**
 * 将ptr指向新的内存块,这是Mem_resize的常见用法,RESIZE可以对ptr多次使用
 */
#define RESIZE(ptr, nbytes) ((ptr) = Mem_resize((ptr), (nbytes), __FILE__, __LINE__))

#endif