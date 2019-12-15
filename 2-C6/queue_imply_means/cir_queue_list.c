#include <string.h>
#include <malloc.h>
#include <iostream>

using namespace std;

typedef struct linkList
{ 
  int   	  data;
  struct linkList *nextNode;
}linkList_t;

typedef struct linkControl
{
  linkList_t *head;
  linkList_t *tail;
}linkControl_t;

void linkList_Init(linkControl_t **lCtrl)
{
  *lCtrl = (linkControl_t *)malloc(sizeof(linkControl_t));
  memset(*lCtrl, 0, sizeof(linkControl_t));
}

bool linkList_addNode(linkControl_t *lCtrl, int nodeData)
{
  linkList_t *newNode = (linkList_t *)malloc(sizeof(linkList_t));
  if(!newNode)
  {
    return false;
  }
  newNode->data = nodeData;
  newNode->nextNode = NULL;
  if((lCtrl->head == NULL) && (lCtrl->tail == NULL))
  {
    lCtrl->head = newNode;			//将newNode地址给lCtrl->head ？？
    lCtrl->tail = newNode;			//两个都传相同的地址,tail如何跳转到数据的尾部？？
  }
  else
  {
    lCtrl->tail->nextNode = newNode;
    lCtrl->tail = newNode;
  }
  return true;
}

bool linkList_removeNode(linkControl_t *lCtrl)
{
  linkList_t *node = lCtrl->head;
  if(node != NULL)
  {
    lCtrl->head = node->nextNode;
    if(node == lCtrl->tail)			//类似于lCtrl->head == lCtrl->tail ,头与尾在同一位置
    {
      lCtrl->tail = NULL;
    }
    if(node)
    {
      free(node);
    }
    return true;
  }
  else 
  {
    return false;
  }
}

void linkList_Deinit(linkControl_t *lCtrl)
{
  linkList_t *tmpP = lCtrl->head;
  linkList_t *tmpQ; 				//少写一个下划线，导致指针错误
  while(tmpP != NULL)
  {
    tmpQ = tmpP;
    tmpP = tmpP->nextNode;
    free(tmpQ);
  }
  if(lCtrl)
  {
    free(lCtrl);
    lCtrl = NULL;
  }
}

void printLinkList(linkControl_t *lCtrl)
{
  linkList_t* tmpP = lCtrl->head;
  while(tmpP != NULL)
  {
    cout << tmpP->data << " ";
    tmpP = tmpP->nextNode;
  }
 cout << endl;
}

int main()
{
  linkControl_t * lCtrl;
  linkList_Init(&lCtrl);
 
  linkList_addNode(lCtrl, 10);
  linkList_addNode(lCtrl, 20);
  linkList_addNode(lCtrl, 30);
  printLinkList(lCtrl);

  linkList_removeNode(lCtrl);
  printLinkList(lCtrl);
  
  linkList_removeNode(lCtrl);
  printLinkList(lCtrl);
  
  linkList_removeNode(lCtrl);
  printLinkList(lCtrl);
  
  linkList_addNode(lCtrl, 40);
  printLinkList(lCtrl);
  
}

