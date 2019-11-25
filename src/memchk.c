#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "mem.h"

#define NDESCRIPTORS 512

//在循环时,如果bp到达freelist,则说明该链表不包含长度大于nbytes的内存块,需要分配一个新的内存块,其长度定义为如下宏
#define NALLOC ((4096 + sizeof(union align) -1) / (sizeof(union align)) * (sizeof(union align)))

void raise_fail(const char * file, int line) {
    if (file == NULL) {
        RAISE(Mem_Failed);
    } else {
        Except_raise(&Mem_Failed, file, line);
    }
}


//------------------------data 54
const Except_T Mem_Failed = {"Allocation Failed"};

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
 * 如果描述符的free字段不是NULL，则该块是空闲
 */
static struct descriptor {
	struct descriptor* free;
	struct descriptor* link;  //link字段构建了一个块描述符的链表，这些块散列到htab中的同一个索引上

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

	while (bp && bp->ptr != ptr) 	{
		bp = bp->link;
	}
	return bp;
}

//内存释放
void Mem_free(void* ptr, const char* file, int line) {
	if (ptr) {
		struct descriptor* bp = NULL;
		//------set bp if ptr is valid 58-----------
		//
		if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||  //对齐过滤
			(bp = find(ptr)) == NULL ||
			bp->free) {
			Except_raise(&Assert_Failed, file, line);
		}
		
		bp->free = freelist.free;
		freelist.free = bp;
	}
}


/**
 *
 * 改变内存大小, 将原来的内存地址数据复制到新的内存块中
 * @param ptr
 * @param nbytes
 * @param file
 * @param line
 * @return
 */
void* Mem_resize(void* ptr, long nbytes, const char* file, int line) {
	struct descriptor* bp = NULL;
	void* newptr = NULL;
	
	assert(ptr);
	assert(nbytes > 0);

	//------set bp if ptr is valid 58-----------
	if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||  //对齐过滤
		(bp = find(ptr)) == NULL || bp->free)	{
		Except_raise(&Assert_Failed, file, line);  //校验地址描述
	}

	newptr = Mem_alloc(nbytes, file, line);  //生成一个新的内存块
	memcpy(newptr, ptr, nbytes < bp->size ? nbytes : bp->size);  //将原来的内存地址数据复制到新的内存块中
	Mem_free(ptr, file, line); //释放原来的内存块
	return newptr;  //返回新内存块地址
}

/**
* 分配地址, 并初始化这块地址
*/
void* Mem_calloc(long count, long nbytes, const char* file, int line){
	void* ptr;

	assert(count > 0);
	assert(nbytes > 0);

	ptr = Mem_alloc(count * nbytes, file, line);
	memset(ptr, '\0', count * nbytes);
	return ptr;
}

/**
* 根据地址指针获得一个地址描述符
*/
static struct descriptor* dalloc(void* ptr, long size, const char* file, int line) {
	static struct descriptor* avail;
	static int nleft;
	if (nleft <= 0) {  //第一次调用dalloc方法时的处理
		//----allocate descriptors 60------------
		avail = malloc(NDESCRIPTORS * sizeof(*avail));
		if (avail == NULL) {
			return NULL;  //内存分配失败,返回给调用者NULL,让调用者去处理
		}
		nleft = NDESCRIPTORS;
	}

	avail->ptr = ptr;
	avail->size = size;
	avail->file = file;
	avail->line = line;
	avail->free = avail->link = NULL;
	nleft--;
	return avail++;
}

void* Mem_alloc(long nbytes, const char* file, int line){
	struct descriptor* bp = NULL;
	void* ptr = NULL; 

	assert(nbytes > 0);
	// -- round nbytes up to an alignment boundary 61---
	//将nbytes向上舍入,使得其返回的每个指针都对齐到联合align大小的倍数
	nbytes = ((nbytes + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align));

	//freelist.free指向空闲链表的起始位置
	for (bp = freelist.free; bp; bp = bp->free) {
		if (bp->size > nbytes) {
			// --use the end of the block at bp->ptr 61--
			//第一个大小大于nbytes的空闲块用来满足该请求. 
			//该空闲块末端nbytes长的空间被切分为一个新块, 在创建其描述符,初始化并添加到htab后,返回该内存块

			bp->size -= nbytes;
			ptr = (char*)bp->ptr + bp->size;  //多出来的一块的地址
			if ((bp = dalloc(ptr, nbytes, file, line)) != NULL) {
				unsigned h = hash(ptr, htab);  //根据地址计算得到索引
				bp->link = htab[h];                 //将该地址的地址描述结构放入到htab中
				htab[h] = bp;
				return ptr;     //返回该地址
			} else {
				//raise Mem_failed 54
				raise_fail(file, line);
			}
		}

		if (bp == &freelist) {
			struct descriptor* newptr;
			// newptr <- a block of size Nalloc + nbytes 62
			//NALLOC 加上nbytes ,该块将添加到空闲链表的起始处,在for循环的下一次迭代中
			if ((ptr = malloc(nbytes + NALLOC)) == NULL 
				|| (newptr = dalloc(ptr, nbytes + NALLOC, __FILE__, __LINE__)) == NULL) {
				raise_fail(file, line);
			}

			newptr->free = freelist.free;
			freelist.free = newptr;
		}
	}
	assert(0);
	return NULL;
}