/* $Id$ */
#ifndef EXCEPT_INCLUDED
#define EXCEPT_INCLUDED
#include <setjmp.h>

#define T Except_T
typedef struct T {
	const char *reason;
} T;

/**
* Except接口中的宏和函数相互协作,维护一个结构栈,栈中的各个结构实例记录了
* 异常状态和实例化的处理程序
*/
typedef struct Except_Frame Except_Frame;
struct Except_Frame {
	Except_Frame *prev;
	jmp_buf env;
	const char *file;
	int line;
	const T *exception;
};


enum { Except_entered=0, Except_raised,
       Except_handled,   Except_finalized };
/**
* Except_stack指向异常栈顶的异常帧,每个帧的prev字段指向前一个帧.
* 引发异常会将异常的地址存储在exception字段中,并将异常的"信息"存储到file和lien字段中.
*/
extern Except_Frame *Except_stack;
extern const Except_T Assert_Failed;

void Except_raise(const T *e, const char *file,int line);


#ifdef WIN32
#include <windows.h>

extern int Except_index;
extern void Except_init(void);
extern void Except_push(Except_Frame *fp);
extern void Except_pop(void);
#endif
#ifdef WIN32
/* $Id$ */
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)
#define RERAISE Except_raise(Except_frame.exception, \
	Except_frame.file, Except_frame.line)
#define RETURN switch (Except_pop(),0) default: return
#define TRY do { \
	volatile int Except_flag; \
	Except_Frame Except_frame; \
	if (Except_index == -1) \
		Except_init(); \
	Except_push(&Except_frame);  \
	Except_flag = setjmp(Except_frame.env); \
	if (Except_flag == Except_entered) {
#define EXCEPT(e) \
		if (Except_flag == Except_entered) Except_pop(); \
	} else if (Except_frame.exception == &(e)) { \
		Except_flag = Except_handled;
#define ELSE \
		if (Except_flag == Except_entered) Except_pop(); \
	} else { \
		Except_flag = Except_handled;
#define FINALLY \
		if (Except_flag == Except_entered) Except_pop(); \
	} { \
		if (Except_flag == Except_entered) \
			Except_flag = Except_finalized;
#define END_TRY \
		if (Except_flag == Except_entered) Except_pop(); \
		} if (Except_flag == Except_raised) RERAISE; \
} while (0)
#else

/**
 * 这个宏用来引发异常 
 */
#define RAISE(e) Except_raise(&(e), __FILE__, __LINE__)

/**
 * 这个宏用来再次引发异常
 * ***/
#define RERAISE Except_raise(Except_frame.exception, \
	Except_frame.file, Except_frame.line)

/**
 在TRY语句内部需要调用return时,用RETURN宏代替
 如果用return语句,会导致一个未检查的运行时错误.
 */	
#define RETURN switch (Except_stack = Except_stack->prev,0) default: return

/**
 * setjmp()第一次返回将Except_flag设置为Except_entered.表示已经进入TRY语句并将一个异常帧压入异常栈
 * Except_entered必须为零,因为第一次调用setjmp()返回0,此后从setjmp返回时会将该标志设置为Except_raised.这表示发生了异常.
 * 处理程序将Except_flag设置为Except_handled,表示以及处理了该异常.
 * 
 * Except_Frame压入异常栈时,只需要将其添加到Except_stack指向的Except_Frame结构链表的头部,而从
 * 链表头部删除异常帧,就表示将栈顶的异常帧出栈
 **/
#define TRY do { \
	volatile int Except_flag; \
	Except_Frame Except_frame; \
	Except_frame.prev = Except_stack; \
	Except_stack = &Except_frame;  \
	Except_flag = setjmp(Except_frame.env); \
	if (Except_flag == Except_entered) {
#define EXCEPT(e) \
		if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
	} else if (Except_frame.exception == &(e)) { \
		Except_flag = Except_handled;
#define ELSE \
		if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
	} else { \
		Except_flag = Except_handled;
#define FINALLY \
		if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
	} { \
		if (Except_flag == Except_entered) \
			Except_flag = Except_finalized;
#define END_TRY \
		if (Except_flag == Except_entered) Except_stack = Except_stack->prev; \
		} if (Except_flag == Except_raised) RERAISE; \
} while (0)
#endif
#undef T
#endif
