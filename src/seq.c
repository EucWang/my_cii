/**
 * Created by wangxn on 2019/11/25.
 *
 * 序列是动态数组的高级抽象
 * 序列包含N个值 分别关联到整数索引 0 ~ N - 1
 * 空序列不包含任何值
 * 类似数组, 序列中的值可以通过索引访问
 * 还可以从序列的两端添加或者删除值.
 * 序列可以根据需要自动扩展, 序列中的值都是指针
 *
 * 序列格式简单,但是可以用作数组, 链表, 栈, 队列 和双端队列,
 * 实现这些数据结构的ADT所需的设施通常都包含在序列中.
 *
 *
 * 序列将 簿记信息和调整大小的相关细节隐藏到了其实现中
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <zconf.h>
#include "assert.h"
#include "seq.h"
#include "array.h"
#include "arrayrep.h"
#include "mem.h"


#define T Seq_T

struct T {
    //array数组用作一个环形缓冲区,以便容纳序列中的值.
    //序列中索引为0的值保存在数组中索引为head的元素处,
    //序列中连续的值保存在数组中"连续"的元素中("连续"是同余意义上的)
    //即若序列中第i个值保存在数组元素array.length-1中, 那么第i+1个值保存在数组元素0中
    struct Array_T array;  // 存储值的数组, array数组至少包含length个元素, 当length小于array.length时,其中一些元素是不适用的
    int length;    // length  字段表示序列中值的数目
    int head;      //
};

//static functions 128

// functions 126

/**
 * 创建并返回一个空序列
 * 无论hint取值如何, 序列都会根据需要扩展以容纳更多内容
 * hint传递负值将抛出异常
 *
 * @param hint hint是序列将包含的最大数目的估计值.  如果该数字未确定, hint可取值0, 以创建一个较小的序列
 * @return
 */
extern T Seq_new(int hint) {
    T seq;

    assert(hint >= 0);

    NEW0(seq);  //初始化length和head字段为0,

    if(hint == 0) {
        hint = 16;
    }

    ArrayRep_init(&seq->array,
            hint,
            sizeof(void *),
            ALLOC(hint * sizeof(void *)));

    return seq;
}

/**
 * 创建并返回一个序列,
 * 用函数的非NULL指针来初始化序列中的值.
 * 参数列表结束于第一个NULL指针参数
 * @param x
 * @param ...
 * @return
 */
extern T Seq_seq(void * x, ...) {
    va_list ap;

    T seq = Seq_new(0);
    va_start(ap, x);
    for(;x; x = va_arg(ap, void *)){
        Seq_addhigh(seq, x);
    }

    va_end(ap);
    return seq;
}


/**
 * 释放空间 seq
 * @param seq
 */
extern void Seq_free(T * seq) {
    assert(seq && *seq);
    assert((void *)*seq == (void *)&(*seq)->array);

    Array_free((Array_T *)seq);
}

/**
 * 返回序列seq中的值的数目
 * @param seq
 * @return
 */
extern int Seq_length(T seq) {
    assert(seq);
    return seq->length;
}

/**
 *  返回序列中的第i个值
 *  i >= seq的length,将导致运行时错误
 * @param seq
 * @param i
 * @return
 */
extern void * Seq_get(T seq, int i) {
    assert(seq);
    assert(i >= 0 && i < seq->length);

    int index = (seq->head + i) % seq->array.length;
    void *pVoid = ((void **) seq->array.array)[index];

    return pVoid;
}

/**
 * 将第i个值改为x,并返回先前的值.
 * i >= seq的length,将导致运行时错误
 * @param seq
 * @param i
 * @param x
 * @return
 */
extern void * Seq_put(T seq, int i, void * x) {

    void * prev;
    assert(seq);
    
    assert(i >= 0 && i < seq->length);

    int index = (seq->head + i) % seq->array.length;
    prev = ((void **) seq->array.array)[index];

    ((void **) seq->array.array)[index] = x;

    return prev;
}


static void expand(T seq) {
    int n = seq->array.length;  //得到数组的大小

    Array_resize(&seq->array, 2 * n); //对数组扩容,并将旧数组中的内容复制到新的数组中

    if(seq->head > 0) {
        // slide tail down 129
        void ** old = &((void **)seq->array.array)[seq->head];

        //将旧数组中的head元素之后的数据向后移动n个位置,这样依然保持数据的环状结构
        memcpy(old + n, old,  (n - seq->head) * sizeof(void *));

        seq->head += n;  //更新head
    }
}

/**
 * 将x添加到seq的低端并返回x
 * 即添加一个值到序列的开始处, 会将所有现存值的索引都+1,
 * 并将序列的长度+1
 *  可能引发Mem_Failed异常
 * @param seq
 * @param x
 * @return
 */
extern void * Seq_addlow(T seq, void * x) {
    int i = 0;
    assert(seq);

    if(seq->length == seq->array.length) {//序列存储的个数已经达到了数组的容量了
        expand(seq); //扩容
    }

    if(--seq->head < 0) {
        seq->head = seq->array.length - 1;
    }

    seq->length ++;

    i = (seq->head) % seq->array.length;

    ((void **) seq->array.array)[i] = x;
    return ((void **) seq->array.array)[i];
}

/**
 *  将x添加到seq的高端处,并返回x
 *  添加一个值到序列的末尾,
 *  并加序列的长度+1
 *  可能引发Mem_Failed异常
 * @param seq
 * @param x
 * @return
 */
extern void * Seq_addhigh(T seq, void * x) {
    int i;

    assert(seq);

    if(seq->length == seq->array.length) {
        expand(seq);
    }

    i = seq->length++;

    int index = (seq->head + i) % seq->array.length;
    ((void **) seq->array.array)[index] = x;
    return ((void **) seq->array.array)[index];
}


/**
 *删除并且返回 序列低端的值, 即在序列的起始位置处删除值
 * 会将余下所有值的序列 - 1
 * 并将序列的长度 -1
 * @param seq  不能为空
 * @return
 */
extern void * Seq_remlow(T seq) {
    int i = 0;
    void * x;

    assert(seq);
    assert(seq->length > 0);

    i = (seq->head) % seq->array.length;
    x = ((void **) seq->array.array)[i];  //获取head位置的值, 但是并没有覆盖这个位置的值

    seq->head = (seq->head + 1) % seq->array.length;  //head值加1

    --seq->length;  //长度减一
    return x;
}


/**
 * 删除并返回序列高端的值
 * 在序列末端删除值, 会将序列的长度 - 1
 *
 * @param seq 不能为空
 * @return
 */
extern void * Seq_remhigh(T seq) {
    int i;
    assert(seq);
    assert(seq->length > 0);

    i = --seq->length;  //长度 -1,

    int index = (seq->head + i) % seq->array.length; //获得该位置的索引
    return ((void **) seq->array.array)[index];
}
