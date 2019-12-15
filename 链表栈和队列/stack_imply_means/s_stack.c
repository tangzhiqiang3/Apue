/* 静态数组实现堆栈程序 stat_stack.c ，数组长度由#define确定 */
#include "stack.h"
#include <stdio.h>
#include <assert.h>

#define STACK_SIZE 100  		//堆栈最大容量元素数量

static	STACK_TYPE stack[STACK_SIZE];
static int top_element = -1;

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
  return top_element == -1;
 // return false
}

/* is_full */
int is_full(void)
{
  return top_element == (STACK_SIZE - 1); 	
}

/* 定义一个print函数，用来打印堆栈里面的元素 */
void print(void)
{
  int i;
  i = top_element;
  printf("打印出静态数组堆栈里面的值：");
  if(i == -1)
    printf("这是个空堆栈\n");
  while(i != -1)
    printf("%d",stack[i--]);
    printf("\n");
}

int main(void)
{
  print();
  push(10); push(9); push(8); push(7); push(6); push(5); push(4); push(3); push(2); push(1); push(0);
  printf("push压入数值后：\n");
  print();
  printf("\n");
  pop();
  pop();
  printf("经过pop弹出几个元素后的堆栈元素：\n");
  print();
  printf("\n");
  printf("top()调用出来的值：%d\n",top());
}


