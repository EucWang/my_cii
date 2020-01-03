/**
 * Created by wangxn on 2020/1/3.
 *
 * 高级字符串
 *
 * Text接口解决了 字符串通常的两个问题:
 * 1. 计算字符串长度花费的时间与字符串长度成正比.
 * 2. 默认的字符串是可以被更改的
 *
 * 解决了这两个问题,即 可以在常数时间内得到字符串长度,
 * 只在必要时才为字符串分配更多内存.
 * Text提供的字符串是不可变的.
 *
 * 因此, Text字符串和C风格的字符串之间需要进行转换,
 * 这些转换函数时Text接口改进带来的代价.
*/

#ifndef ICD_TEXT_H
#define ICD_TEXT_H

#include <stdarg.h>

/**
 * Text_T指向的字符串可以包括任何字符, 包括0字符
 *
 * 不允许改变Text_T中的字符串, 除非通过本接口中定义的函数
 * 这里的函数都是按值传递和返回描述符, 而不是指向描述符的指针.
 * Text函数都不分配描述符.
 *
 * Text通过自身来分配字符串空间,
 * 不能在外部释放字符串
 */
typedef struct T {
    int len;           //字符串的长度
    const char * str;  //这里的str字段指向的字符串不是以0字符作为结尾的
} T;

#define T Text_T

// -- exported types 192


// -- exported data 195

/**
 * 由所有256个8比特字符组成的字符串
 */
extern const T Text_cset;

/**
 * 包含128个ASCII字符
 */
extern const T Text_ascii;

/**
 * 大写字母组成的字符串
 */
extern const T Text_ucase;

/**
 * 小写字母组成的字符串
 */
extern const T Text_lcase;

/**
 * 数字0~9组成的字符串
 */
extern const T Text_digits;

/**
 * 空串
 */
extern const T Text_null;

// -- exported functions 193

/**
 * 将c风格字符串转换成Text结构体
 * @param str  c风格的字符串   str为NULL则会导致运行时异常
 * @return   Text结构体
 */
extern T Text_put(const char * str);

/**
 * Text_get将s描述的字符串复制到str[0.. size-2]中,附加一个0字符,并返回str
 * @param str    如果str是NULL, Text_get忽略size, 调用Mem_alloc分配s.len+1个字节,将Text复制到新空间中并返回
 * @param size 长度为size-1个字符从Text中复制到str字符串中, 以0字符结尾
 * @param s Text结构体
 * @return
 */
extern char * Text_get(char * str, int size, T s);

/**
 * 为常数字符串建立Text结构,
 * @param str
 * @param len
 * @return
 */
extern T Text_box(const char * str, int len);

/**
 * Text_sub没有为返回值分配新的空间, 子串和原来的Text共享一段空间
 *
 * @param s
 * @param i 位置i  , 正顺序从1开始, 逆顺序从0开始
 * @param j 位置j
 * @return 返回一个Text子串
 */
extern T Text_sub(T s, int i, int j);

/**
 * 返回s的Text结构中对应于任意位置i的正数位置
 * @param s  Text结构体
 * @param i 任意位置i
 * @return
 */
extern int Text_pos(T s, int i);

/**
 * 连接两个Text结构体的字符串 并返回
 * 如果s1或者s2是空串, 则返回另一个参数
 * 仅在必要时,Text_cat才会创建一个新的副本
 * @param s1
 * @param s2
 * @return
 */
extern T Text_cat(T s1, T s2);

/**
 * 复制Text结构s重复n次得到一个一个新的Text
 * @param s
 * @param n   不能为负值,否则导致运行时错误
 * @return
 */
extern T Text_dup(T s, int n);

/**
 * 翻转Text结构中的字符串并返回一个新的Text
 * @param s
 * @return
 */
extern T Text_reverse(T s);

/**
 * 返回根据from和to指向的字符串隐射s的结果, 隐射规则如下:
 * 对于s中每个出现在from中的字符, 使用to中对应字符替换后输出到结果字符串
 * 对于s中没有出现在from中的字符, 字符本身直接输出到结果字符串.
 *
 * 如果from和to都是NULL, 则使用记录的值.
 * 如果from和to中仅有一个为NULL,或者二者均非NULL时 如果from->len != to->len, 则会导致运行时异常
 *
 * @param s
 * @param from
 * @param to
 * @return
 */
extern T Text_map(T s, const T * from, const T * to);

/**
 * 字符串比较函数
 * @param s1
 * @param s2
 * @return  当s1小于,等于,大于s2时, 该函数分别返回 负值, 0, 正值
 */
extern int Text_cmp(T s1, T s2);

//---------------------------------------------------
//下列函数是一组字符串分析函数, 与Str接口导出的相关函数几乎相同.

/**
 * 在 s[i:j]查找左侧的字符c
 * @param s Text
 * @param i 位置而不是索引
 * @param j 位置而不是索引
 * @param c 待查找的字符
 * @return  返回s中该字符左侧的正数位置, 没找到则返回0
 */
extern int Text_chr(T s, int i, int j, int c);

/**
 * 在 s[i:j]查找右侧的字符c
 * @param s Text
 * @param i 位置而不是索引
 * @param j 位置而不是索引
 * @param c 待查找的字符
 * @return  返回s中该字符左侧的正数位置, 没找到则返回0
 */
extern int Text_rchr(T s, int i, int j, int c);

/**
 * 在s[i:j]中从左向右搜索set中的任意字符
 * 返回找到的第一个字符左侧的位置, 没找到返回0
 * @param s
 * @param i
 * @param j
 * @param set
 * @return
 */
extern int Text_upto(T s, int i, int j, T set);

/**
 *
 * 在s[i:j]中从右向左搜索set中的任意字符
 * 返回找到的第一个字符左侧的位置, 没找到返回0
 * @param s
 * @param i
 * @param j
 * @param set
 * @return
 */
extern int Text_rupto(T s, int i, int j, T set);

/**
 * TODO
 * @param s
 * @param i
 * @param set
 * @return
 */
extern int Text_any(T s, int i, T set);

/**
 * 如果s[i:j]以set中的某个字符开始, Text_many返回完全由set中字符组成的(最长)
 * 子串之后的正数位置, 否则返回0
 * @param s
 * @param i
 * @param j
 * @param set
 * @return
 */
extern int Text_many(T s, int i, int j, T set);

/**
 * 如果s[i:j]以set中的某个字符结束, Text_rmany返回完全由set中字符组成的(最长)
 * 子串之前的正数位置, 否则返回0
 * @param s
 * @param i
 * @param j
 * @param set
 * @return
 */
extern int Text_rmany(T s, int i, int j, T set);


////////////////////////////////////////////////////////////////////////////
// 剩下的分析函数用于查找字符串

/**
 * 在s[i:j]最左侧查找字符串str子串
 * @param s
 * @param i
 * @param j
 * @param str
 * @return
 */
extern int Text_find(T s, int i, int j, T str);

/**
 * 在s[i:j]最右侧查找字符串str子串
 * @param s
 * @param i
 * @param j
 * @param str
 * @return
 */
extern int Text_rfind(T s, int i, int j, T str);

extern int Text_match(T s, int i, int j, T str);

extern int Text_rmatch(T s, int i, int j, T str);


#endif //ICD_TEXT_H
