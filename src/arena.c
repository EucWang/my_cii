//
// Created by wangxn on 2019/10/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "assert.h"
#include "except.h"
#include "arena.h"

#define T Arena_T

// 定义最大的freechunks链表上停留的空闲内存的大小
// 在nfree值达到THRESHOLD后,后续到达的大内存块将通过free释放
#define THRESHOLD 10

const Except_T Arena_NewFailed = {"Arena Creation Failed."};

const Except_T Arena_Failed = {"Arena Allocation Failed."};

//--macros 71--

//--types 67--
union align{
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

/**
 * 一个内存池描述了一大块内存
 * 每一块新分配的内存块,开始位置,首先都是放置的这么一个结构体,
 * 每一个新分配的内存块,在申请内存成功之后,都会放到内存池链表的头部的下一个位置,
 */
struct T {
    T prev;       //指向下一个内存块的起始位置
    char *avail;  //指下一个内存块的开始空闲位置,从avail开始,到limit之前的空间都可以用于分配
    char *limit;  //指下一个内存块的结束处
};

union header {
    struct T b;
    union align a;
};

//--data 70--

/**
 * 真实的再内存中分配一块内存
 * @param nbytes  分配的内存块大小
 * @param file
 * @param line
 * @param ptr
 * @param limit
 */
void real_alloc_memory(long nbytes, const char *file, int line, Arena_T *ptr, char **limit);

/**
 * 一个空闲链表,维护被释放了的大内存块
 * 该链表将大内存块头部的Arena_T结构实例的prev字段用作链表指针,
 * 这些结构实例的limit字段只是指向其所处大内存块的结束处.
 */
static T freechunks;
/**
 * nfree是链表中大内存块的数目.
 */
static int nfree;


static Arena_Status arenaStatus;


//--functions 68--

/**
 * 分配一个空的内存池
 * 各个字段都为空
 */
T Arena_new(void) {

    T arena = malloc(sizeof(*arena));
    if(arena == NULL) {
        RAISE(Arena_NewFailed);
    }
    arena->prev = NULL;
    arena->limit = arena->avail = NULL;

    arenaStatus = malloc(sizeof(*arenaStatus));
    arenaStatus->totalMem = sizeof(*arena);
    arenaStatus->usedMem = sizeof(*arena);
    arenaStatus->freeMem = 0L;
    return arena;
}


/**
 * 释放内存池,需要将其中的大内存块添加到空闲大内存块的链表中,
 * 这里会遍历内存池中的大内存块链表,
 * @param arena
 */
void Arena_free(T arena) {
    assert(arena);
    while(arena->prev) {
        struct T tmp = *arena->prev;    //结构赋值,注意这里的方式不一样
        //--free the chunk described by arena 71
        if(nfree < THRESHOLD) {
            arena->prev->prev = freechunks;   //指针赋值
            freechunks = arena->prev;         //指针赋值
            nfree++;
            freechunks->limit = arena->limit;  // 这里不需要处理arena->avail字段,因为这是空闲的内存块, 注意,这里的limit是指下一块内存块的limit.

            arenaStatus->freeMem += ((char *)arena->limit - (char *)arena->prev);
        } else {
            long dm = ((char *)arena->limit - (char *)arena->prev);
            arenaStatus->totalMem -= dm;
            arenaStatus->blocks--;

            free(arena->prev);
        }

        *arena = tmp;
    }
    arenaStatus->usedMem = 0;
    assert(arena->limit == NULL);
    assert(arena->avail == NULL);
}

/**
 * 清空内存池
 * @param ap 内存池指针
 */
void Arena_dispose(T * ap) {
    assert(ap && *ap);
    Arena_free(*ap);
    free(*ap);
    free(arenaStatus);
    *ap = NULL;
}

void * Arena_alloc(T arena, long nbytes, const char * file, int line) {
    printf("Arena_alloc() calling.. param nbytes = %d\n", nbytes);
    assert(arena);
    assert(nbytes > 0);

    //--round nbytes up to an alignment boundary 69--
    long align_size = sizeof(union align);
    nbytes = ((nbytes + align_size - 1)/align_size) * align_size; //得到对齐的长度
    printf("Arena_alloc() calulate nbytes = %d\n", nbytes);

    T ptr, tmp;
    char * limit;

    bool find = false;
    tmp = arena;
    while ((ptr = tmp) != NULL) { //循环遍历正在使用的区块链表, 查找可用的内存块,如果找到,则跳出循环
        if((tmp->limit - tmp->avail) > nbytes){
            limit = ptr->limit;
            find  = true;
            break;
        } else {
            tmp = tmp->prev;
        }
    }

    if (find) {    //如果找到了
        printf("Arena_alloc() find out from arena.\n");
        ptr->avail += nbytes;  //相当于前nbytes个字节的数据已经被分配了
        arenaStatus->usedMem += nbytes;
        return ptr->avail - nbytes;  //返回这块被分配了的数据库的地址
    }

    //从正在使用的区块链表中没有找到,则循环遍历空闲区块
    if(!find) {
        tmp = freechunks;
        while ((ptr = tmp) != NULL) {   //循环遍历空闲区块链表, 找到可用的区块
            if((tmp->limit - (char *)((union header *)tmp + 1)) > nbytes){
                limit = ptr->limit;
                find  = true;

                //从空闲内存池链表中移除该内存块
                if (tmp->prev){
                    tmp->limit = tmp->prev->limit;
                    tmp->prev = tmp->prev->prev;
                }
                nfree--;

                break;
            } else {
                tmp = freechunks->prev;
            }
        }
    }

    //从空闲区块链表和正在使用的区块链表中都没有找到,则分配一块新的内存空间
    if(!find) {
        //分配新的区块,放入到内存池链表中
        real_alloc_memory(nbytes, file, line, &ptr, &limit);
        arenaStatus->usedMem += sizeof(*arena);
        find = true;
        printf("Arena_alloc() donot find out from freechunks, then alloc from primary memory.\n");
    }

    if(find) {
        //将*arena 下推, 保存在新的大内存块的起始处,header联合确保了arena->avail指向一个适合对齐的地址
        //*arena 的值就是结构体Arena_T,ptr开始位置存储的也是Arena_T结构体,这里就是结构体赋值,新生产的大内存开始位置存储内存池链表头的数据
        *ptr = *arena;
        //将内存池链表头指向新生成的内存块
        arena->avail = (char *)((union header *)ptr + 1);
        arena->limit = limit;
        arena->prev = ptr;
    }

    arena->avail += nbytes;  //相当于前nbytes个字节的数据已经被分配了
    arenaStatus->usedMem += nbytes;
    return arena->avail - nbytes;  //返回这块被分配了的数据库的地址
}

/**
 * 
 * @param arena 
 * @param nbytes 
 * @param file 
 * @param line 
 * @return 
 */
void * Arena_alloc1(T arena, long nbytes, const char * file, int line) {
    assert(arena);
    assert(nbytes > 0);
    
    //--round nbytes up to an alignment boundary 69--
    long align_size = sizeof(union align);
    nbytes = ((nbytes + align_size - 1)/align_size) * align_size; //得到对齐的长度
    
    //这里的while循环在分配失败时会再次尝试进行分配,如果新的大内存块来自于freechunks
    while(nbytes > arena->limit - arena->avail) {  //需要分配一个新的大内存块
        //--get a new chunk 69--
        T ptr;
        char * limit;
        //--ptr <- a new chunk 70
        if((ptr = freechunks) != NULL) {
            freechunks = freechunks->prev;
            nfree--;
            limit = ptr->limit;
        } else {
            //TODO update 这里需要遍历整个内存池链表之后,才去分配新的内存块
            real_alloc_memory(nbytes, file, line, &ptr, &limit);
        }

        //将*arena 下推, 保存在新的大内存块的起始处,header联合确保了arena->avail指向一个适合对齐的地址
        //*arena 的值就是结构体Arena_T,ptr开始位置存储的也是Arena_T结构体,这里就是结构体赋值,新生产的大内存开始位置存储内存池链表头的数据
        *ptr = *arena;
        //将内存池链表头指向新生成的内存块
        arena->avail = (char *)((union header *)ptr + 1);
        arena->limit = limit;
        arena->prev = ptr;
    }

    arena->avail += nbytes;  //相当于前nbytes个字节的数据已经被分配了
    return arena->avail - nbytes;  //返回这块被分配了的数据库的地址
}

void real_alloc_memory(long nbytes, const char *file, int line, Arena_T *ptr, char **limit) {
    printf("real_alloc_memory() calling..\n");
    long m = sizeof(union header) + nbytes + 10 * 1024;
    printf("real_alloc_memory() calculate m = %ld\n", m);
    (*ptr) = malloc(m);
    if((*ptr) == NULL) {
        //--raise Arena_Failed 70
        if(file == NULL) {
            RAISE(Arena_Failed);
        } else {
            Except_raise(&Arena_Failed, file, line);
        }
    }
    (*limit) = (char *) (*ptr) + m;
    arenaStatus->totalMem += m;
    arenaStatus->usedMem += sizeof(union header);
    arenaStatus->blocks ++;
}

/**
 * 
 * @param arena 
 * @param count 
 * @param nbytes 
 * @param file 
 * @param line 
 * @return 
 */
void * Arena_calloc(T arena, long count, long nbytes, const char * file, int line) {
    printf("Arena_calloc() calling and count = %d, nbytes = %d .\n", count, nbytes);
    void * ptr;
    assert(count > 0);
    ptr = Arena_alloc(arena, count * nbytes, file, line);
    memset(ptr, '\0', count*nbytes);
    return ptr;
}

Arena_Status Arena_status(){
    return arenaStatus;
}

#undef T