//static char rcsid[] = "$Id$";
#include "assert.h"
#include "except.h"

const Except_T Assert_Failed = { "Assertion failed" };

/**
 * 在这里, 围绕函数名assert的括号防止宏assert在此展开,因而按接口的规定定义了该函数.
 * 
 * 如果客户程序没有处理Assert_Failed,那么断言失败将导致程序放弃执行,并输出一条信息:
 * Uncaught exception Assertion failed raised at stmt.c:201
 * aborting...
 **/
void (assert)(int e) {
	assert(e);
}
