/*
** 单链表实现堆栈，没有长度限制
*/
#include "stack.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define FALSE 0

/*
** 定义一个结构以存储堆栈元素
*/

typedef struct STACK_NODE
{
  STACK_TYPE         value;
  struct STACK_NODE  *next; 
}StackNode;

/* 指向堆栈中第一个节点的指针 */
static StackNode *stack;

/* creat_stack */
void creat_stack(size_t size)
{}

/* destroy_stack */
void destroy_stack(void)
{
  while(!is_empty())
   pop();
}

/* push */
void push(STACK_TYPE value)
{
  StackNode  *new_node;
  new_node=(StackNode *)malloc(sizeof(StackNode));
  if(new_node == NULL)
   perror("malloc fail");
  new_node->value = value; 		//传值给新节点
  new_node->next = stack;		//将首地址传给新链表的指向的下个节点
  stack = new_node;			//将stack指向新链表，即将新链表插入头部
}

/* pop */
void pop(void)
{
  StackNode  *first_node;
  assert(!is_empty());
  first_node = stack;
  stack = first_node->next;
  free(first_node);

}

/* top */
STACK_TYPE top(void)
{
  assert(!is_empty());
  return stack->value;
}

/* is_empty */
int is_empty(void)
{
  return stack == NULL;
}

/* is_full */
int is_full(void)
{
  return FALSE;
}

/*
** 定义一个print函数，打印堆栈里面的元素
*/
void print(void)
{
  StackNode *p_node;
  p_node = stack;
  printf("打印出链式堆栈里面的值: ");
  if(p_node == NULL)
   printf("堆栈为空\n");
  while(p_node != NULL)
   {
     printf("%d ",p_node->value);
     p_node = p_node->next;
   }
   printf("\n");
}

int main(void)
{
  print();
  push(10); push(9); push(8); push(7); push(6); push(5); push(4); push(3); push(2); push(1); push(0);
  printf("push压入堆栈后的值为: ");
  print();
  printf("\n");
  pop();
  pop();
  printf("经过pop弹出几个元素后的堆栈元素：\n");
  print();
  printf("\n");
  printf("top()调用出来的值: %d\n",top());
  destroy_stack();
  return 1;
}

