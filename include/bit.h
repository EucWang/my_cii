/**
 * Created by wangxn on 2019/11/27.
 *
 * Bit接口提供了操作位向量的函数
 *
 */

#ifndef ICD_BIT_H
#define ICD_BIT_H

#define T Bit_T
typedef  struct T * T;

/**
 * Bit_new创建一个包含length个比特位的新向量,并将所有比特位都设置为0.
 * 该向量表示了从0到length-1 的所有整数(包含0 和 length-1)
 * @param length 不能为负数,为负数则导致运行时异常
 * @return Bit_T
 */
extern T Bit_new(int length);

/**
 * 返回Bit_T中的比特位数.
 * @param set   不能为NULL, 为NULL会导致运行时异常
 * @return
 */
extern int Bit_length(T set);

/**
 * 返回Bit_T中1个数目, (即置位的比特位数)
 * @param set 不能为NULL, 为NULL会导致运行时异常
 * @return
 */
extern int Bit_count(T set);


/**
 * 释放 *set 并将*set置为NULL
 * @param set   不能为NULL, 为NULL会导致运行时异常
 */
extern void Bit_free(T * set);

/**
 * 返回比特位n,
 *
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param n   不能为负值, 不能大于等于set的长度,否则会导致运行时错误
 * @return
 */
extern int Bit_get(T set, int n);

/**
 * 将比特位n设置为bit,并返回该比特位的原值.
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param n    不能为负值或者是大于等于set的长度,否则导致运行时异常
 * @param bit  不能是0和1以外的值,否则导致运行时错误
 * @return
 */
extern int Bit_put(T set, int n, int bit);

/**
 * Bit_clear将low到high的所有比特位清零, 包含low和high比特位
 * @param set   不能为NULL,,否则会导致运行时错误
 * @param low   low不能大于high, 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 * @param high  不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 */
extern void Bit_clear(T set, int low, int high);

/**
 * Bit_set将low到high的所有比特位置 置为位1 , 含比特位low和high
 * @param set 不能为NULL,,否则会导致运行时错误
 * @param low
 * @param high
 */
extern void Bit_set(T set, int low, int high);

/**
 * Bit_not将low到high所有比特位取反
 * @param set  不能为NULL,,否则会导致运行时错误
 * @param low  low不能大于high, 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 * @param high 不能为负值,low/high 不能大于等于set的长度,否则会导致运行时错误
 */
extern void Bit_not(T set, int low ,int high);

/**
 *  如果s是t的一个真子集, 返回1, 否则返0
 * @param s 不能为NULL,,否则会导致运行时错误
 * @param t 不能为NULL,,否则会导致运行时错误, s和t的长度必须相同,否则会导致运行时错误
 * @return
 */
extern int Bit_lt(T s, T t);

/**
 *如果s == t, 则Bit_eq返回1, 否则返回0
 * @param s   如果s或者t为NULL, 或者s的长度和t的长度不同,则会导致运行时异常
 * @param t
 * @return  返回 1: 相同, 0 不相同
 */
extern int Bit_eq(T s, T t);

/**
 * 如果s是t的一个子集, 返回1, 否则返回0
 * @param s  如果s或者t为NULL, 或者s的长度和t的长度不同,则会导致运行时异常
 * @param t
 * @return
 */
extern int Bit_leq(T s, T t);

/**
 * 从比特位0开始, 对set中的每一个比特位调用apply, n是比特位的编号
 * 介于0和集合的长度减1之间, bit是比特位n的值, cl有客户程序提供.
 * apply可以改变set本身
 * @param set
 * @param apply   apply不同于Table_map的函数, 这里可以改变set.若比特位n调用apply时,apply改变了比特位k,且k>n,则到k时修改可见
 * @param cl
 */
extern void Bit_map(T set, void apply(int n, int bit, void * cl), void * cl);

/**
 * 返回s和t的并集 s + t, 实际上就是两个位向量的可兼容的按位或操作
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_union(T s, T t);

/**
 * 返回s和t的交集 s * t, 就是两个位向量的按位与
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_inter(T s, T t);

/**
 * 返回s和t的差集 (s - t), 是t的补集和s按位与
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_minus(T s, T t);

/**
 * 返回s和t的对称差 s/t, 就是两个位向量的按位异或
 * @param s  s或t可以为NULL指针,但是不能同时为NULL; 若同时为NULL,或者长度不同,会导致运行时异常
 * @param t
 * @return  返回非NULL的Bit_T
 */
extern T Bit_diff(T s, T t);

#undef T
#endif //ICD_BIT_H
