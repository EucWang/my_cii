/**
 * Created by wangxn on 2019/12/17.
*/
#include <stdarg.h>
//#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
//#include <math.h>

#include "fmt.h"
#include "assert.h"
#include "except.h"
#include "mem.h"

/**
 * 调用put(c)函数n此,
 */
#define pad(n, c) do { int nn = (n); \
    while(nn-- > 0) \
    put((c), cl);} while(0)


// types 162

/**
 * Fmt_vsfmt会初始化该结构的一个局部变量,
 * 并将该变量的实例指针传递给Fmt_vfmt
 */
struct buf {
    char * buf;  //复制的Fmt_vsfmt的名称类似的参数
    char * bp;   // bp指向buf中输出下一个被格式化字符的位置
    int size;    //复制的Fmt_vsfmt的名称类似的参数
};

// macros 160
// conversion functions 159


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
static void Fmt_putd(const char * str,
                     int len,
                     int put(int c, void * cl),
                     void * cl,
                     unsigned char flags[256],
                     int width,
                     int precision) {
    assert(str);
    assert(len >= 0);
    assert(flags);

    int sign;

    //normalize width and flags 159
    // ==
    // normalize width 160
    if (width == INT_MIN) {
        width= 0;
    }
    if (width < 0) {
        flags['-'] = 1;
        width = - width;
    }
    // normalize flags 160
    if (precision >= 0) {
        flags['0'] = 0;
    }

    //compute the sign 167
    if(len > 0 && (*str == '-' || *str == '+')) {
        sign = *str++;
        len--;
    } else if(flags['+']) {
        sign = '+';
    } else if(flags[' ']) {
        sign = ' ';
    } else {
        sign = 0;
    }

    // --emit str justified in width 167 --
    int n;
    if(precision < 0) {
        precision = 1;
    }
    if(len < precision) {
        n = precision;
    } else if(precision == 0 && len == 1 && str[0] == '0') {
        n = 0;
    } else {
        n = len;
    }
    if (sign) {
        n++;
    }

    if (flags['-']) {
        //emit the sign 168
        if(sign) put(sign, cl);
    } else if(flags['0']) {
        //emit the sign 168
        if(sign) put(sign, cl);
        pad(width - n, '0');
    } else {
        pad(width-n, ' ');
        //emit the sign 168
        if(sign) put(sign, cl);
    }

    pad(precision - len, '0');
    //emit str[0.. len-1] 159
    int i;
    for(i = 0; i< len; i++) {
        put((unsigned char) * str ++, cl);
    }

    if(flags['-']) pad(width - n, ' ');
}

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
static void Fmt_puts(const char * str,
              int len,
              int put(int c, void *cl),
              void * cl,
              unsigned char flags[256],
              int width,
              int precision) {

    assert(str);
    assert(len >= 0);
    assert(flags);

    //normalize width and flags 159
    // ==
    // normalize width 160
    if (width == INT_MIN) {
        width= 0;
    }
    if (width < 0) {
        flags['-'] = 1;
        width = - width;
    }
    // normalize flags 160
    if (precision >= 0) {
        flags['0'] = 0;
    }

    if (precision >= 0 && precision < len) {
        len = precision;
    }

    if (!flags['-']) {
        pad(width - len, ' ');
    }

    //emit str[0 .. len - 1] 159
    int i;
    for(i = 0; i< len; i++) {
        put((unsigned char) * str ++, cl);
    }

    if (flags['-']) {
        pad(width - len , ' ');
    }
}

static void cvt_o(int code,
        va_list *app,
        int put(int c, void * cl),
        void *cl,
        unsigned char flags[],
        int width,
        int precision) {

    unsigned m = va_arg(*app, unsigned);

    //declare buf and p, initialize p 166
    char buf[43];
    char * p = buf + sizeof(buf);

    do {
        *--p = (m&0x7) + '0';
    } while((m >>= 3) != 0);

    Fmt_putd(p, (buf + sizeof(buf)) - p, put, cl, flags, width, precision);

}

/**
 *
 * @param code
 * @param app
 * @param put
 * @param cl
 * @param flags
 * @param width
 * @param precision
 */
static void cvt_x(int code,
        va_list *app,
        int put(int c, void *cl),
        void *cl,
        unsigned char flags[],
        int width,
        int precision) {

    unsigned m = va_arg(*app,unsigned);

    //declare buf and p, initialize p 166
    char buf[43];
    char * p = buf + sizeof(buf);

    //emit m in hexadecimal 169
    do {
        *--p = "0123456789abcdef"[m&0xf];
    } while((m>>= 4) != 0);

    Fmt_putd(p, (buf + sizeof(buf)) - p, put, cl, flags, width, precision);
}

static void cvt_p(int code,
        va_list *app,
        int put(int c, void * cl),
        void *cl,
        unsigned char flags[],
        int width,
        int precision) {

    unsigned long m = (unsigned long)va_arg(*app, void *);

    //declare buf and p, initialize p 166
    char buf[43];
    char * p = buf + sizeof(buf);

    precision = INT_MIN;

    //emit m in hexadecimal 169
    do {
        *--p = "0123456789abcdef"[m&0xf];
    } while((m>>= 4) != 0);

    Fmt_putd(p, (buf + sizeof(buf)) - p, put, cl, flags, width, precision);
}


/**
*  对应于 %s 的转换函数
* @param code
* @param app
* @param put
* @param cl
* @param flags
* @param width
* @param precision
*/
static void cvt_s(int code,
                  va_list *app,
                  int put(int c, void * cl),
                  void *cl,
                  unsigned char flags[],
                  int width,
                  int precision) {

    char * str = va_arg(*app, char *);
    assert(str);
    int len = strlen(str);
    Fmt_puts(str, len, put, cl, flags, width, precision);
}

/**
 * 相对于cvt_d要简单,
 * 但它可以使用Fmt_putd输出转换结果的所有机制.
 * 它将输出下一个无符号整数的十进制表示
 * @param code
 * @param app
 * @param put
 * @param cl
 * @param flags
 * @param width
 * @param precision
 */
static void cvt_u(int code,
        va_list *app,
        int put(int c, void *cl),
        void * cl,
        unsigned char flags[],
        int width,
        int precision) {

    unsigned m = va_arg(*app, unsigned);

    //declare buf and p, initialize p 166
    char buf[43];
    char * p = buf + sizeof(buf);

    do {
        *--p = m%10 + '0';
    } while((m/=10) > 0);

    Fmt_putd(p, (buf + sizeof(buf)) - p, put, cl, flags, width, precision);
}

/**
 * cvt_d是对应于%d的转换函数,
 * 在格式化数字的转换函数中具有代表性.
 *
 * @param code
 * @param app
 * @param put
 * @param cl
 * @param flags
 * @param width
 * @param precision
 */
static void cvt_d(int code,
        va_list * app,
        int put(int c, void * cl),
        void *cl,
        unsigned char flags[],
        int width,
        int precision) {

    int val = va_arg(*app, int);
    unsigned m;

    //declare buf and p, initialize p 166
    char buf[43];
    char * p = buf + sizeof(buf);

    if(val == INT_MIN) {
        m = INT_MAX + 1U;
    } else if(val < 0) {
        m = -val;
    } else {
        m = val;
    }
    do {
        *--p = m%10 + '0';
    }while((m /= 10) > 0);

    if(val < 0) {
        *--p = '-';
    }

    Fmt_putd(p , (buf + sizeof(buf)) - p, put, cl, flags, width, precision);
}

static void cvt_f(int code,
        va_list *app,
        int put(int c, void *cl),
        void *cl,
        unsigned char flags[],
        int width,
        int precision) {

    char buf[DBL_MAX_10_EXP + 1 + 1 + 99 + 1];

    if (precision < 0) {
        precision = 6;
    }

    if (code == 'g' && precision == 0) {
        precision = 1;
    }

    //format a double argument into buf 170
    static char fmt[] = "%.dd?";
    assert(precision <= 99);
    fmt[4] = code;
    fmt[3] = precision % 10 + '0';
    fmt[2] = (precision / 10) % 10 + '0';
    sprintf(buf, fmt, va_arg(*app, double));

    Fmt_putd(buf, strlen(buf), put , cl, flags, width, precision);
}

/**
 * 与%c相关的转换函数
 * @param code
 * @param app
 * @param put
 * @param cl
 * @param flags
 * @param width
 * @param precision
 */
static void cvt_c(int code,
                  va_list *app,
                  int put(int c, void *cl),
                  void *cl,
                  unsigned char flags[],
                  int width,
                  int precision) {

    //normalize width 160
    if (width == INT_MIN) {
        width= 0;
    }
    if (width < 0) {
        flags['-'] = 1;
        width = - width;
    }

    if(!flags['-']) {
        pad(width - 1, ' ');
    }

    put((unsigned char)va_arg(*app, int), cl);
    if(flags['-']) {
        pad(width - 1, ' ');
    }
}

// data 160

char *Fmt_flags = "-+ 0";

static T cvt[256] = {
        /*0~7*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*8~15*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*16~23*/ 0, 0, 0, 0, 0, 0, 0, 0,

        /*24~31*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*32~39*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*40~47*/ 0, 0, 0, 0, 0, 0, 0, 0,

        /*48~55*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*56~63*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*64~71*/ 0, 0, 0, 0, 0, 0, 0, 0,

        /*72~79*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*80~87*/ 0, 0, 0, 0, 0, 0, 0, 0,
        /*88~95*/ 0, 0, 0, 0, 0, 0, 0, 0,

        /*96~103*/ 0, 0, 0, cvt_c, cvt_d, cvt_f, cvt_f, cvt_f,
        /*104~111*/ 0, 0, 0, 0, 0, 0, 0, cvt_o,
        /*112~119*/ cvt_p, 0, 0, cvt_s, 0, cvt_u, 0, 0,

        /*120~127*/ cvt_x, 0, 0, 0, 0, 0, 0, 0
};

/**
 * 定义了特定的格式化函数的 异常
 */
const Except_T Fmt_Overflow = {"Formatting Overflow"};

// static functions 161

/**
 * 类似于Fmt_vsfmt的put, 只是会在必要时将buf的容量加倍,
 * 使之能够容纳格式化输出的字符
 * @param c
 * @param cl
 * @return
 */
static int append(int c, void *cl) {
    struct buf * p = cl;

    if(p->bp >= p->buf + p->size) {
        RESIZE(p->buf, 2 * p->size);
        p->bp = p->buf + p->size;
        p->size *= 2;
    }
    (*p->bp ++) = c;
    return c;
}

/**
 * insert函数会检查是否有空间容纳需要输出的字符,
 * 并将该字符存储到bp字段指向的位置.
 * 并将bp字段 + 1
 *
 * @param c
 * @param cl 类型是 struct buf
 * @return
 */
static int insert(int c, void * cl) {
    struct buf * p = cl;
    if(p->bp >= p->buf + p->size) {  //检查是否超出返回,如果超出就抛出运行时异常
        RAISE(Fmt_Overflow);
    }
    *p->bp ++ = c;
    return c;
}

/**
 * outc作为 Fmt_print 和 Fmt_fprint 的入参,
 * outc 作为put函数使用,
 * outc内部实际调用put标准io函数
 * @param c
 * @param cl
 * @return
 */
static int outc(int c, void * cl) {
    FILE * f = cl;
    return putc(c, f);
}

// functions 159


/**
 * Fmt_vfmt是实现的核心, 所有其他接口函数都是调用它来完成实际的格式化工作.
 *
 * Fmt_vfmt按照fmt给出的格式串来格式化ap指向的各个参数,
 * 具体过程类似于Fmt_fmt
 * @param put  put函数返回一个整数,通常是其参数, 可以使用标准I/O函数fputc
 * @param cl  直接传递给put()
 * @param fmt
 * @param ap
 */
static void Fmt_vfmt(int put(int c, void * cl),
                     void * cl,
                     const char *fmt,
                     va_list *ap) {

    assert(put);
    assert(fmt);

    while(*fmt) {
        if(*fmt != '%' || *++fmt == '%') {
            put((unsigned char )*fmt++, cl);
        } else {
            //逐一处理各个标志,字段宽度, 精度设置,
            //以及处理转换限定符没有对应的转换函数的可能性
            //format an argument 164

            unsigned char c, flags[256];
            int width = INT_MIN;
            int precision = INT_MIN;

            memset(flags, '\0', sizeof flags);

            //在参数指定了宽度或者精度时, 其值不能为INT_MIN,该值是保留的, 作为默认值.
            //在宽度或者精度显式的给定时,它不能大于INT_MAX,即等效于约束 10 * n + d <= INT_MAX
            //即10 * n + d不会上溢.
            //在不导致上溢的情况下进行该测试.
            //get optional flags 165
            if(Fmt_flags) {
                unsigned char c = *fmt;
                //char *strchr(const char *str, int c) 在参数 str 所指向的字符串中搜索第一次出现字符 c（一个无符号字符）的位置。
                for(; c && strchr(Fmt_flags, c); c = *++fmt) {
                    assert(flags[c] < 255);
                    flags[c]++;
                }
            }

            //get optional field width 165
            if (*fmt == '*' || isdigit(*fmt)) {
                int n;
                //n <- next argument or scan digits 165
                if (*fmt == '*') {
                    n = va_arg(*ap , int);
                    assert(n != INT_MIN);
                    fmt++;
                } else {
                    for (n = 0; isdigit(*fmt); fmt++) {
                        int d = *fmt - '0';
                        assert(n <= (INT_MAX - d)/10);
                        n = 10 * n + d;
                    }
                }

                width = n;
            }

            //句点表明接下来是一个可选的精度设置
            //注意,句点如果没有后接星号或者数字, 那么将处理以及解释为显式忽略的精度.
            //get optional precision 166
            if(*fmt == '.' &&
               (*++fmt == '*' || isdigit(*fmt))) {
                int n;
                //n <- next argument or scan digits 165
                if (*fmt == '*') {
                    n = va_arg(*ap , int);
                    assert(n != INT_MIN);
                    fmt++;
                } else {
                    for (n = 0; isdigit(*fmt); fmt++) {
                        int d = *fmt - '0';
                        assert(n <= (INT_MAX - d)/10);
                        n = 10 * n + d;
                    }
                }

                precision = n;
            }

            c = *fmt++;
            assert(cvt[c]);
            (*cvt[c])(c, ap, put, cl, flags, width, precision);
        }
    }
}

/**
 * 返回格式化之后的字符串
 * 需要自己负责释放返回的字符串
 * 可能引发Mem_Failed异常
 * @param fmt
 * @param ap
 * @return
 */
static char * Fmt_vstring(const char * fmt, va_list * ap) {
    assert(fmt);

    struct buf cl;
    cl.size = 256;
    cl.buf = cl.bp = ALLOC(cl.size);

    Fmt_vfmt(append, &cl, fmt, ap);
    append(0, &cl);
    return RESIZE(cl.buf, cl.bp - cl.buf);
}


/**
 * 类似于C库中的vsprintf() 将格式化输出以\0结尾的字符串形式中
 *
 * @param buf
 * @param size
 * @param fmt
 * @param ap
 * @return
 */
static int Fmt_vsfmt(char * buf, int size, const char * fmt, va_list * ap) {
    assert(buf);
    assert(size > 0);
    assert(fmt);

    struct buf cl;
    cl.buf = cl.bp = buf;
    cl.size = size;
    Fmt_vfmt(insert, &cl, fmt, ap);  //调用Fmt_vfmt
    insert(0, &cl);
    return cl.bp - cl.buf - 1;
}

/**
 * Fmt_T定义了转换函数的签名
 */
//#define T Fmt_T

//typedef void (*T)(int code,
//                  va_list * ap,
//                  int put(int c, void *cl),
//                  void *cl,
//                  unsigned char flags[256],
//                  int width,
//                  int precision);

//extern char * Fmt_flags;

//exported functions 155

/**
 * Fmt_fmt按照第三个参数fmt给出的格式串来格式化其第四个和后续参数
 * 并调用put(c, cl)来输出每个格式化完毕的字符c, c当做unsigned char 处理
 * @param put  put函数返回一个整数,通常是其参数, 可以使用标准I/O函数fputc
 * @param cl   直接传递给put()
 * @param fmt  参数列表
 * @param ...
 */
extern void Fmt_fmt(int put(int c, void * cl),
                    void * cl,
                    const char *fmt, ...) {

    va_list ap;
    va_start(ap, fmt);
    Fmt_vfmt(put, cl, fmt, &ap);
    va_end(ap);
}


/**
 * 类似于C库中的printf()  将格式化输出写到标准输出
 * @param fmt
 * @param ...
 */
extern void Fmt_print(const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Fmt_vfmt(outc, stdout, fmt, &ap);
    va_end(ap);
}

/**
 * 类似于C库中的fprintf() 将格式化输出写到指定的输出流
 *
 * @param stream
 * @param fmt
 * @param ...
 */
extern void Fmt_fprint(FILE * stream, const char * fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Fmt_vfmt(outc, stream, fmt, &ap);
    va_end(ap);
}

/**
 * 类似于C库中的sprintf() 将格式化输出以\0结尾的字符串形式中
 *
 * @param buf
 * @param size
 * @param fmt
 * @param ...
 * @return
 */
extern int Fmt_sfmt(char * buf, int size, const char * fmt, ...) {
    int len;

    va_list ap;
    va_start(ap, fmt);

    len = Fmt_vsfmt(buf, size, fmt, &ap);
    va_end(ap);
    return len;
}

/**
 * 返回格式化之后的字符串
 * 需要自己负责释放返回的字符串
 * 可能引发Mem_Failed异常
 * @param fmt
 * @param ...
 * @return
 */
extern char * Fmt_string(const char * fmt, ...) {
    char * str;
    assert(fmt);

    va_list ap;
    va_start(ap, fmt);
    str = Fmt_vstring(fmt, &ap);
    va_end(ap);

    return str;
}


/**
 * 每个格式符C都关联到一个转换函数, 这些关联可以通过调用本函数来改变
 * Fmt_register()将cvt设置为code指定的格式符对应的转换函数,
 * 并返回指向先前转换函数的指针.
 * 所以, 客户程序可以临时替换转换函数, 而后又恢复到原来的转换函数.
 *
 * Fmt_register通过将cvt中适当的元素设置为相应的函数指针,
 * 来设置一个新的转换函数. 并返回该元素的原值:
 *
 * @param code   取值  255 >= code >= 1
 * @param cvt
 * @return
 */
extern T Fmt_register(int code, T newcvt) {
    T old;

    assert( code > 0 &&
        code < (int)(sizeof(cvt)/sizeof(cvt[0])));

    old = cvt[code];
    cvt[code] = newcvt;
    return old;
}


