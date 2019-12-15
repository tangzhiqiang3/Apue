/* 堆栈模块的接口 stack.h */
#include<stdlib.h>
#define STACK_TYPE int  //堆栈所存储的值的数据类型

/*
** 函数原型：create_stack
** 创建堆栈，参数指定堆栈可以保存多个元素
** 注意：此函数只适用于动态分配数组形式的堆栈
*/
void create_stack(size_t size);


/*
** 函数原型：destroy_stack
** 销毁一个堆栈，释放堆栈所适用的内存
** 注意：此函数只适用于动态分配数组和链式结构的堆栈
*/
void destroy_stack(void);


/*
** 函数原型：push
** 将一个新值压入堆栈中，参数是被压入的值。
*/
void push(STACK_TYPE value);


/*
** 函数原型：pop
** 弹出堆栈中的栈顶的一个值，并丢弃
*/
void pop(void);


/*
** 函数原型：top
** 返回堆栈顶部元素的值，但不改变堆栈结构
*/
STACK_TYPE top(void);


/*
** 函数原型：is_empty
** 如果堆栈为空，返回TURE，否则返回FALSE
*/
int is_empty(void);


/*
** 函数原型：is_full
** 如果堆栈为空，返回TURE，否则返回FALSE
*/
int is_full(void);

