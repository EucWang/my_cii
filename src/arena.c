//
// Created by wangxn on 2019/10/15.
//

#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "except.h"
#include "arena.h"

#define T Arena_T

const Except_T Arena_NewFailed = {"Arena Creation Failed."};

const Except_T Arena_Failed = {"Arena Allocation Failed."};

//--macros 71--

//--types 67--
/**
 * 一个内存池描述了一大块内存
 */
struct T {
    T prev;     //内存池起始位置
    char *avail;  //指向大内存中第一个空闲位置,从avail开始,到limit之前的空间都可以用于分配
    char *limit;  //指向内存池的结束处
};

//--data 70--

//--functions 68--

#undef T