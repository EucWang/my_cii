/**
 * Created by wangxn on 2019/12/17.
 *
 * 格式化函数
*/

#ifndef ICD_FMT_H
#define ICD_FMT_H

#include <stdarg.h>
#include <stdio.h>
#include "except.h"

/**
 * Fmt_T定义了转换函数的签名
 */
#define T Fmt_T

typedef void (*T)(int code,
                  va_list * ap,
                  int put(int c, void *cl),
                  void *cl,
                  unsigned char flags[256],
                  int width,
                  int precision);

extern char * Fmt_flags;

/**
 * 定义了特定的格式化函数的 异常
 */
extern const Except_T Fmt_Overflow;

//exported functions 155

/**
 * Fmt_fmt按照第三个参数fmt给出的格式串来格式化其第四个和后续参数
 * 并调用put(c, cl)来输出每个格式化完毕的字符c, c当做unsigned char 处理
 * 第一个参数可以使用 : 对fputc进行函数签名转换是必须的, 原本的fputc的类型是 : int (*)(int , FILE*)
 * 第二个参数可以使用 stdout
 * 第三个参数开始就可以是需要格式化的字符串了
 *
 * @param put  put函数返回一个整数,通常是其参数, 可以使用标准I/O函数fputc
 * @param cl   直接传递给put()
 * @param fmt  参数列表
 * @param ...
 */
extern void Fmt_fmt(int put(int c, void * cl),
                    void * cl,
                    const char *fmt, ...);

/**
 * Fmt_vfmt按照fmt给出的格式串来格式化ap指向的各个参数,
 * 具体过程类似于Fmt_fmt
 * @param put  put函数返回一个整数,通常是其参数, 可以使用标准I/O函数fputc
 * @param cl  直接传递给put()
 * @param fmt
 * @param ap
 */
//extern void Fmt_vfmt(int put(int c, void * cl),
//                    void * cl,
//                    const char *fmt,
//                    va_list *ap);

/**
 * 类似于C库中的printf()  将格式化输出写到标准输出
 * @param fmt
 * @param ...
 */
extern void Fmt_print(const char * fmt, ...);

/**
 * 类似于C库中的fprintf() 将格式化输出写到指定的输出流
 *
 * @param stream
 * @param fmt
 * @param ...
 */
extern void Fmt_fprint(FILE * stream, const char * fmt, ...);

/**
 * 类似于C库中的sprintf() 将格式化输出以\0结尾的字符串形式中
 *
 * @param buf    接受格式化之后的字符串
 * @param size   buf的可容纳长度
 * @param fmt    需被格式化的字符串内容
 * @param ...    替换格式化
 * @return
 */
extern int Fmt_sfmt(char * buf, int size, const char * fmt, ...);

///**
// * 类似于C库中的vsprintf() 将格式化输出以\0结尾的字符串形式中
// *
// * @param buf
// * @param size
// * @param fmt
// * @param ap
// * @return
// */
//extern int Fmt_vsfmt(char * buf, int size, const char * fmt, va_list ap);

/**
 * 返回格式化之后的字符串
 * 需要自己负责释放返回的字符串
 * 可能引发Mem_Failed异常
 * @param fmt
 * @param ...
 * @return
 */
extern char * Fmt_string(const char * fmt, ...);

/**
 * 每个格式符C都关联到一个转换函数, 这些关联可以通过调用本函数来改变
 * Fmt_register()将cvt设置为code指定的格式符对应的转换函数,
 * 并返回指向先前转换函数的指针.
 * 所以, 客户程序可以临时替换转换函数, 而后又恢复到原来的转换函数.
 * @param code   取值  255 >= code >= 1
 * @param cvt
 * @return
 */
extern T Fmt_register(int code, T cvt);

/**
 * 许多转换函数, 都是%d 和 %s 转换限定符对应的转换函数的变体.
 * 本函数是数值的内部转换函数
 * 本函数假定 str[0.. len-1] 包含了一个有符号数的字符串表示
 * 它将按照flags, width, precision指定的转换%d, 并输出字符串
 * Fmt_putd本身不是转换函数, 但是可以可以被转换函数调用
 *
 * @param str  为NULL时,会导致运行时异常 ; 指向可变长度参数列表指针的指针, 用于访问被格式化的数据
 * @param len   len < 0时,会导致运行时异常 ; 格式码
 * @param put    为NULL时,会导致运行时异常 ; 输出函数
 * @param cl                            ; 输出函数的相关数据
 * @param flags  为NULL时,会导致运行时异常 ; 标志
 * @param width                         ;字段宽度
 * @param precision                     ; 精度
 */
//extern void Fmt_putd(const char * str,  /*指向可变长度参数列表指针的指针, 用于访问被格式化的数据*/
//        int len,                        /*格式码*/
//        int put(int c, void * cl),      /*输出函数*/
//        void * cl,                      /*输出函数的相关数据*/
//        unsigned char flags[256],       /*标志*/
//        int width,                      /*字段宽度*/
//        int precision);                 /*精度*/

/**
 * 许多转换函数, 都是%d 和 %s 转换限定符对应的转换函数的变体.
 * 本函数是数字符串的内部转换函数
 * 按照flags, width, precision指定的转换%s, 来输出 str[0.. len-1]
 * *Fmt_puts本身不是转换函数, 但是可以可以被转换函数调用
 *
 * @param str   为NULL时,会导致运行时异常 ; 指向可变长度参数列表指针的指针, 用于访问被格式化的数据
 * @param len    len < 0时,会导致运行时异常 ; 格式码
 * @param put     为NULL时,会导致运行时异常 ; 输出函数
 * @param cl       输出函数的相关数据
 * @param flags   为NULL时,会导致运行时异常; 标志, 字符数组中第i个元素等于标志字符i在转换限定符中出现的次数
 * @param width     字段宽度, 没有显式给出时取值 INT_MIN
 * @param precision   精度, 没有显式给出时取值 INT_MIN
 */
extern void Fmt_puts(const char * str,  /*指向可变长度参数列表指针的指针, 用于访问被格式化的数据*/
        int len,                        /*格式码*/
        int put(int c, void *cl),       /*输出函数*/
        void * cl,                     /*输出函数的相关数据*/
        unsigned char flags[256],       /*标志, 字符数组中第i个元素等于标志字符i在转换限定符中出现的次数*/
        int width,                      /*字段宽度*/
        int precision);                 /*精度*/

#endif //ICD_FMT_H
