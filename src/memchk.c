#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

/**
* 如果Mem_alloc, Mem_calloc, Mem_resize从来不把同一个地址返回两次,
* 且能够记录所有返回的地址以及那些地址指向已分配的内存,
* 那么Mem_free , Mem_resize 就可以检测内存访问错误.
* 所以, 内存操作的函数需要维护一个集合, 
* 其元素为对(a, free) 或者 (a, allocated), 
* a 表示地址, free表示地址已经不可用, allocated表示地址已分配
* Mem_alloc, Mem_calloc 会添加对 (ptr, allocated) 到集合中, 且保证添加前集合中不存在.
* 
* 返回的ptr为NULL或者在集合中为(ptr, allocated), 则Mem_free(ptr)是合法的;
* 返回的ptr不为NULL且在集合中为(ptr, allocated),则Mem_free(ptr)将释放内存,并改变集合中对应项为(ptr, free)
* 
* 仅当(ptr, allocated)在集合中,Mem_resize才是合法的.
* 
***/

//  ----------------   checking types 58

//  ----------------   checking macros 58

//  ----------------   data 54

//  ----------------   checking data 56

//  ----------------   checking functions 58

