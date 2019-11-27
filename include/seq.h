/**
 * // Created by wangxn on 2019/11/25.
 *
 * 序列是动态数组的高级抽象
 * 所以序列包含一个动态数组,
 * 这不是指向Array_T的一个指针,
 * 而是Array_T结构体本身的一个实例
 */

#ifndef ICD_SEQ_H
#define ICD_SEQ_H

#define T Seq_T

typedef struct T * T;

/**
 * 创建并返回一个空序列
 * 无论hint取值如何, 序列都会根据需要扩展以容纳更多内容
 * hint传递负值将抛出异常
 *
 * @param hint hint是序列将包含的最大数目的估计值.  如果该数字未确定, hint可取值0, 以创建一个较小的序列
 * @return
 */
extern T Seq_new(int hint);

/**
 * 这个方法调用时,最后一个元素最好是NULL,没有NULL也要加一个NULL
 *
 * 创建并返回一个序列,
 * 用函数的非NULL指针来初始化序列中的值.
 * 参数列表结束于第一个NULL指针参数
 * @param x
 * @param ...
 * @return
 */
extern T Seq_seq(void * x, ...);


/**
 * 释放空间 seq
 * @param seq
 */
extern void Seq_free(T * seq);

/**
 * 返回序列seq中的值的数目
 * @param seq
 * @return
 */
extern int Seq_length(T seq);

/**
 *  返回序列中的第i个值
 *  i >= seq的length,将导致运行时错误
 * @param seq
 * @param i
 * @return
 */
extern void * Seq_get(T seq, int i);

/**
 * 将第i个值改为x,并返回先前的值.
 * i >= seq的length,将导致运行时错误
 * @param seq
 * @param i
 * @param x
 * @return
 */
extern void * Seq_put(T seq, int i, void * x);

/**
 * 将x添加到seq的低端并返回x
 * 即添加一个值到序列的开始处, 会将所有现存值的索引都+1,
 * 并将序列的长度+1
 *  可能引发Mem_Failed异常
 * @param seq
 * @param x
 * @return
 */
extern void * Seq_addlow(T seq, void * x);

/**
 *  将x添加到seq的高端处,并返回x
 *  添加一个值到序列的末尾,
 *  并加序列的长度+1
 *  可能引发Mem_Failed异常
 * @param seq
 * @param x
 * @return
 */
extern void * Seq_addhigh(T seq, void * x);

/**
 *删除并且返回 序列低端的值, 即在序列的起始位置处删除值
 * 会将余下所有值的序列 - 1
 * 并将序列的长度 -1
 * @param seq  不能为空
 * @return
 */
extern void * Seq_remlow(T seq);

/**
 * 删除并返回序列高端的值
 * 在序列末端删除值, 会将序列的长度 - 1
 *
 *
 * @param seq 不能为空
 * @return
 */
extern void * Seq_remhigh(T seq);

#undef T
#endif //ICD_SEQ_H
