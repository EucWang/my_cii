/**
 * Created by wangxn on 2019/11/27.
 *
 * 环和序列非常相似, 包含N个值,
 * 分别关联到整数索引 0 ~ N-1
 * 空的环不包含任何值
 * 环的值都是指针
 * 与序列中的值类似,
 * 环中的值也可以用索引访问
 */

#ifndef ICD_RING_H
#define ICD_RING_H



#define T Ring_T

typedef struct T * T;

/**
 * 创建并返回一个空环
 * @return
 */
extern T Ring_new(void);

/**
 * 创建并返回一个环,
 * 参数列表结束于第一个NULL指针参数,
 * 所以最后一个参数必须是NULL
 * @param x
 * @param ...
 * @return
 */
extern T Ring_ring(void * x, ...);

/**
 * 释放*ring指定的环并将其清零
 * 如果ring或者*ring是NULL,则导致运行时错误
 *
 * @param ring
 */
extern void Ring_free(T * ring);

/**
 * 返回ring中值的数目
 * @param ring  为NULL导致运行时错误
 * @return
 */
extern int Ring_length(T ring);

/**
 * 返回index索引处的值
 * @param ring   为NULL导致运行时错误
 * @param index  小于0或者大于ring的值的长度导致运行时错误,即 取值为 0 ~ N-1
 * @return
 */
extern void *Ring_get(T ring, int index);

/**
 * 将ring中索引值为index的值修改为val,并返回原始值
 * @param ring     为NULL导致运行时错误
 * @param index    小于0或者大于ring的值的长度导致运行时错误 ,即 取值为 0 ~ N-1
 * @param val      这里可以为NULL,没有做检查
 * @return
 */
extern void * Ring_put(T ring, int index, void * val);


/**
 * 将val值添加到ring中的pos位置处,并返回val
 * 在一个含有N个值的环中, pos位置指定了值之间的地点,
 * 添加一个新值,会将其右侧所有值的索引加1,并将环的长度加1.
 * 可能导致Mem_Failed
 * 如下, 给出了一个包含6个值的环的正负位置:
 *
 *
 *  1   2   3   4   5   6   7     <-- 这是正位置,pos, 取值 1 ~ (n+1)
 *  |   |   |   |   |   |   |
 *  V   V   V   V   V   V   V
 *  -------------------------
 *  | 0 | 1 | 2 | 3 | 4 | 5 |    <-- 这是索引值 index
 *  -------------------------
 *  ^   ^   ^   ^   ^   ^   ^
 *  |   |   |   |   |   |   |
 * -6  -5  -4  -3  -2  -1   0     <-- 这是负位置, pos, 取值 -n ~ 0
 *
 *
 * @param  ring  为NULL导致运行时错误
 * @param  pos   大于 N+1 或者小于-N 导致运行时错误
 * @param  val   这里可以为NULL,没有做检查
 * @return
 */
extern void * Ring_add(T ring, int pos , void * val);


/**
 * Ring_addlow()函数等效于 Ring_add(ring, 1, val)
 * @param ring
 * @param val
 * @return
 */
extern void * Ring_addlow(T ring, void * val);

/**
 * Ring_addhigh()等效于Ring_add(ring, 0, val)
 * @param ring
 * @param val
 * @return
 */
extern void * Ring_addhigh(T ring ,void * val);

/**
 * 删除并返回ring中的索引为index的元素的值,
 * 删除之后,会将其右侧剩下的索引都减1, 并将环的长度减小1,
 *
 * @param ring    为NULL导致运行时错误
 * @param index   小于0或者大于ring的长度, 会导致运行时错误
 * @return
 */
extern void * Ring_remove(T ring, int index);

/**
 * 删除并返回位于ring的低端的值
 * 等效于 Ring_remove(ring, 0)
 *
 * @param ring
 * @return
 */
extern void * Ring_remlow(T ring);

/**
 * 删除并返回位于ring的高端的值
 * 等效于 Ring_remove(ring, Ring_length(ring) - 1)
 *
 * @param ring
 * @return
 */
extern void * Ring_remhigh(T ring);

/**
 * 左旋或者右旋 ring, 将其中的值重新编号
 * 如果n为正值,ring则右旋转n个值(顺时针), 各个值的索引加n然后对ring的长度取模
 * 如果n为负值, ring向左旋转n个值(逆时针),各个值的索引值减n然后对环的长度取模.
 * 如果环的长度为0, 无效果
 * 如果n的绝对值大于ring的长度, 会导致运行时错误
 *
 * @param ring
 * @param n
 */
extern void Ring_rotate(T ring , int n);

#undef T
#endif //ICD_RING_H
