#ifndef ASSERT_H_
#define ASSERT_H_
/* $Id$ */
#undef assert
// #define NDEBUG  开启这端,那么assert()断言无效
#ifdef NDEBUG
    #define assert(e) ((void)0)
#else
    #include "except.h"
    extern void assert(int e);

    // 类似 e1 || e2 的表达式通常出现在条件判断中,
    // 如if语句
    // 但它也可以作为单独的语句出现.作为单独语句,该表达式的效果等效于:
    // if(!(e1)) e2;   
    // assert(e) 只能扩展为表达式,而不能是语句,所以这里 使用了 e1||e2.
    //整个表达式最终转换为void, 因为c标准规定assert(e)没有返回值.

    //这里
    #define assert(e) ((void)((e)||(RAISE(Assert_Failed),0)))
#endif

#endif