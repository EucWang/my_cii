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

//用于对齐的联合体，
// Mem_alloc返回的指针，地址值总是对其到下列联合的大小倍数。
//这种对其确保了任何类型的数据都可以保存在Mem_alloc返回的块中。
//如果传递给Mem_freede ptr不符合这种对齐，它不可能在htab中
union align
{
	int i;
	long l;
	long* lp;
	void* p;
	void (*fp)(void);
	float f;
	double d;
	long	double ld;
};


//  ----------------   checking macros 58

/**
	hash宏将地址作为一个位模式处理，右移三位，并将其对htab的大小取模，以减小其值
*/
#define hash(p, t) (((unsigned long)(p)>>3) & (sizeof (t)/sizeof((t)[0]) -1))

//  ----------------   data 54

//  ----------------   checking data 56

/**
 * htab包含了所有块的描述符，包括空闲块和已分配块， 同时空闲块还同时出现在freelist链表上
 * 如果描述符的free字段为NULL，则该块以及分配，
 * 如果描述符的free字段不是NULL，则该块是空闲的， 
 *
 * link字段构建了一个块描述符的链表，这些块散列到htab中的同一个索引上
*/
static struct descriptor {
	struct descriptor* free;
	struct descriptor* link;  //

	const void* ptr;  //块的地址，在代码的其他地方分配
	long size;            //块的长度
	const char* file;  //调用相关函数的源代码文件
	int line;               //调用相关函数的源代码行数
} * htab[2048];

/**
	一个环形链表， freelist是链表中最后一个描述符，
	其free字段指向链表中第一个描述符。
*/
static struct descriptor freelist = { &freelist };



//  ----------------   checking functions 58

//根据指针找到其在散列表中的描述符
static struct descriptor* find(const void* ptr) {
	struct descriptor* bp = htab[hash(ptr, htab)];

	while (bp && bp->ptr != ptr) {
		bp = bp->link;
	}
	return bp;
}

//内存释放
void Mem_free(void* ptr, const char* file, int line) 
{
	if (ptr)
	{
		struct descriptor* bp = NULL;
		//------set bp if ptr is valid 58-----------
		//
		if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||  //对齐过滤
			(bp = find(ptr)) == NULL ||
			bp->free)
		{
			Except_raise(&Assert_Failed, file, line);
		}


		bp->free = freelist.free;
		freelist.free = bp;
	}
}


//改变内存大小
void* Mem_resize(void* ptr, long nbytes, const char* file, int line)
{
	struct descriptor* bp = NULL;
	void* newptr = NULL;
	
	assert(ptr);
	assert(nbytes > 0);

	//------set bp if ptr is valid 58-----------
	if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||  //对齐过滤
		(bp = find(ptr)) == NULL ||
		bp->free)
	{
		Except_raise(&Assert_Failed, file, line);
	}

	newptr = Mem_alloc(nbytes, file, line);
	memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);
	Mem_free(ptr, file, line);
	return newptr;
}

void* Mem_calloc(long count, long nbytes, const char* file, int line)
{
	void* ptr;

	assert(count > 0);
	assert(nbytes > 0);

	ptr = Mem_alloc(count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);
	return ptr;
}

void* Mem_alloc(long nbytes, const char* file, int line)
{
	//TODO
	struct descriptor* bp = NULL;
	void* ptr = NULL; 

	return ptr;
}