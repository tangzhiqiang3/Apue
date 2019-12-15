/*
** 动态分配数组实现的堆栈程序 d_stack.c
** 堆栈长度在创建堆栈的函数被调用时给出，该函数必须在任何其他操作堆栈的函数被调用之前调用
*/
#include "stack.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

/*
** 用于存储堆栈元素的数组和指向堆栈顶部元素的指针
*/


static STACK_TYPE *stack;
static int 	  stack_size;
static int	  top_element = -1;

/* create_stack */
void create_stack(size_t size)
{
  assert(stack_size == 0);
  stack_size = size;
  stack = (STACK_TYPE *)malloc(stack_size * sizeof(STACK_TYPE));
  if(stack == NULL)
  perror("malloc分配失败: ");
}

/* destroy */
void destroy_stack(void)
{
  assert(stack_size > 0);
  stack_size = 0;
  free(stack);
  stack = NULL;
}

/* push */
void push(STACK_TYPE value)
{
  assert(!is_full());
  top_element += 1;
  stack[top_element] = value;
}

/* pop */
void pop(void)
{
  assert(!is_empty());
  top_element -= 1;
}

/* top */
STACK_TYPE top(void)
{
  assert(!is_empty());
  return stack[top_element];
}

/* is_empty */
int is_empty(void)
{
  assert(stack_size > 0);
  return top_element == -1;
}

/* is_full */
int is_full(void)
{
  assert(stack_size > 0);
  return top_element == stack_size - 1;
}

/*
** 定义一个print函数，用来打印堆栈里面的元素
*/
void print(void)
{
  int i;
  i = top_element;
  printf("打印出动态数组堆栈里面的值：");
  if(i == -1)
  printf("这是一个空堆栈\n");
  while(i != 1)
  printf("%d ",stack[i--]);
  printf("\n");
}
int main(void)
{
  create_stack(50);
  push(10); push(9); push(8); push(7); push(6); push(5); push(4); push(3); push(2); push(1);
  printf("push压入数值后: \n");
  print();
  pop();
  pop();
  printf("经过pop弹出几个元素后的堆栈元素: \n");
  print();
  printf("\n");
  printf("top()调用出来的值为: %d\n",top());
  destroy_stack();
  return 1;
}


