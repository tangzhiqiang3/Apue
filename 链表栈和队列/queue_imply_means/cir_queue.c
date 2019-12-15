/*
** 使用的函数需要声明，可将声明放在一个新建的头文件中，也可将其在该源码中直接声明
*/
#include <string.h>
#include <malloc.h>
#include <iostream>
//#include "circQUe.h"

#define MAX_NODE_NUM 10

using namespace std;

typedef struct cirQueue
{
  int front;
  int rear;
  char data[MAX_NODE_NUM];
}circQue_t;

void circQue_Init(circQue_t *queue)
{
  queue->front = 0;
  queue->rear = 0;
  memset(queue->data, 0, MAX_NODE_NUM);
}

void circQue_Deinit(circQue_t *queue)
{
  if(queue)
  { 
    cout << "will free queue,front is:" << queue->front <<",rear is:" << queue->rear <<endl;
    free(queue);
  }
}

bool isQueEmpty(circQue_t *queue)
{
  return (queue->front == queue->rear); 		//queue为空时，返回ture，否则false
}

bool isQueFull(circQue_t *queue)
{
  return (((queue->rear+1)%MAX_NODE_NUM == queue->front));
}

bool circQue_add(circQue_t *queue, char newData)
{
  if(isQueFull(queue))
  {
    cout<< "queue is full, add node fail!" << endl;
    return false;
  }
  queue->data[queue->rear] = newData;
  queue->rear = (queue->rear+1)%MAX_NODE_NUM;
  cout << "front is:" << queue->front << ",rear is:" << queue->rear << endl;
  return true;
}

char circQue_remove(circQue_t *queue)
{
  if(isQueEmpty(queue))
  {
    cout<< "queue is empty, remove node fail!" << endl;
    return false;  
  }
  char nodeValue = queue->data[queue->front];		//将数据另存，为了传参
  queue->front = (queue->front+1)%MAX_NODE_NUM;   	//将移除头部数据，front向后移,即加1
  return nodeValue;
}

void printQueueData(circQue_t *queue)
{
  for(int i = queue->front; i != queue->rear; i=(i+1)%MAX_NODE_NUM)
  {
    cout<< queue->data[i] << "";
  }
  cout<< endl;
}

int main()
{
  circQue_t * que = (circQue_t *)malloc(sizeof(circQue_t));
  if(!que)
  {
    cout<< "malloc for circular queue failed!" << endl;
    return -1;
  }
  circQue_Init(que);
  circQue_add(que, 'a');
  circQue_add(que, 'b');
  circQue_add(que, 'c');
  printQueueData(que);
  circQue_remove(que);
  printQueueData(que);
 
}


