#include "stdio.h"
#include <stdlib.h>
#include "string.h"

typedef int elemType;
typedef struct Node
{ 
  elemType      element;
  struct Node * next;
}Node;


/* 1.初始化线性表，即置单链表的表头指针为空 */
void initList(Node **pNode)
{ 
  *pNode = NULL;
 // memset(*pNode,0,sizeof(Node);)
  printf("initList函数执行，初始化成功\n");

}

/* 2.创建线性链表，此函数输入负数终止读取数据 */
Node *creatList(Node *pHead)
{
 Node *p1;
 Node *p2;
 p1=p2=(Node *)malloc(sizeof(Node));
 if(p1==NULL||p2==NULL)
 {
  printf("内存分配失败\n");
  exit(0);
 }
 memset(p1, 0, sizeof(Node));	
 scanf("%d",&p1->element); 	//输入新节点
 p1->next=NULL; 		//新节点指针置为空
 while(p1->element > 0)  	//输入的值大于0，则继续，直到输入的值为负
 {
  if(pHead == NULL)		//空表，接入表头
  {
    pHead = p1;
  }
  else
  {
    p2->next = p1;   		//非空表，接入表尾
  }
  p2 = p1;
  p1=(Node *)malloc(sizeof(Node));//再重新申请一个节点
  if(p1==NULL||p2==NULL)
  {
    printf("内存分配失败\n");
    exit(0);
  }
  memset(p1,0,sizeof(Node));
  scanf("%d",&p1->element);
  p1->next=NULL; 
 }
 printf("creatList函数执行，链表创建成功\n");
 return pHead;  		//返回链表的头指针
}

/* 3.打印链表，链表的遍历 */
void printList(Node *pNode)
{
  Node *p1;
  while(pNode!=NULL)
  {
    //p1 = pNode->element;
    printf("printList函数执行，链表元素为：%d",p1->element);
    //if(pNode->next=NULL)
    //{
      pNode=pNode->next;
    //}
  }
  printf("printList函数执行，该链表为空\n");
}

/* 4.清除线性表L中的所有元素，即释放单链表L中所有节点，使之成为一个空链表 */
void clearList(Node *pHead)
{
  Node *ptime				//存放临时数据
  while(pHead->next!=NULL)  			//释放节点，所以只看节点是否为空
  {
    ptime = pHead->next;			//保存下一个节点的指针
    //memset(pNode,0,sizeof(Node));     //将所有元素清理，不是释放！
    free(pHead);  			//清除内存单元
    pHead = ptime;			//表头下移
    //exti(0);
  }    
  printf("clearList函数执行，清除链表成功\n");
  //return (0)
}

/* 5.返回单链表长度 */
int sizeList(Node *pHead)
{
  int i=0;
  while(pHead!=NULL) 
  {
    i++;
    pHead=pHead->next;
  }
  printf("sizeList函数执行，单链表长度%d\n",i);
  return i;				//定义的是int 有返回值
}

/* 6.返回单链表中第pos个结点中的元素，若pos超出范围，则停止程序运行 */
elemType getElement(Node *pHead, int pos)
{ int i=0;
  if(pos<1)
  {
    printf("pos值非法");
    exti(1);
  }
  if(pHead==NULL)
  {
    printf("getElement函数执行，链表为空\n");
  }
  while(pHead!=NULL)
  {
   i++;
   //if(i<pos)
   //{
     pHead=pHead->next;
   //}
   if(i=pos)
   {
    break;
    // return pHead->element;
   }
  }
  if(i<pos)
  {
    printf("getElement函数执行，pos超出范围\n");
    return(0);
  }
   return pHead->element;
}

/* 7.从单链表中查找具有给定值x的第一个元素，若查找成功则返回该结点data域的存储地址，否则返回NULL */
elemType *getElemAddr(Node *pHead, int x)
{
  if(pHead==NULL)
  { 
    printf("getElemaddr函数执行，链表为空\n");
    return NULL;
  }
  if(x < 0)
  {
     printf("getElemAddr函数执行，给定值X不合法\n");
     return NULL;
  }
  while((pHead->next != NULL)&&(pHead->element != x))
  {
    //if(pHead->element==x)
    pHead=pHead->next;
  }
  if((pHead->element != x)&&(pHead->next != NULL))
  {
     printf("getElemAddr函数执行，在链表中未找到x值\n");
     return NULL;
  }
  if(pHead->element == x)
  {
     printf("getElemAddr函数执行，元素%d的地址为0x%x\n",x,&(pHead->element));
  }
  return &(pHead->element);

}

/* 8.向链表头插一个元素 */
int insertHeadList(Node **pNode, elemType insertElem)
{
  Node *pInsert;
  //if(pHead==NULL)
  //{     
  //   printf("getElemaddr函数执行，链表为空\n");
  //   return NULL;
  //}
  pInsert=(Node *)malloc(sizeof(Node));
  memset(pInsert,0,sizeof(Node));
  pInsert->element=insertElem;
  pInsert->next = *pNode;
  *pNode=pInsert;
  printf("insertHeadList函数执行，向表头插入元素成功\n");
  return 1;
}


int main()
{
    Node *pList=NULL;
    int length = 0;
 
    elemType posElem;
 
    initincluList(&pList);       //链表初始化
    printList(pList);       //遍历链表，打印链表
 
    pList=creatList(pList); //创建链表
    printList(pList);
     
    sizeList(pList);        //链表的长度
    printList(pList);
 
    //isEmptyList(pList);     //判断链表是否为空链表
     
    posElem = getElement(pList,3);  //获取第三个元素，如果元素不足3个，则返回0
    printf("getElement函数执行，位置 3 中的元素为 %d\n",posElem);   
    printList(pList);
 
    getElemAddr(pList,5);   //获得元素5的地址
 
    modifyElem(pList,4,1);  //将链表中位置4上的元素修改为1
    printList(pList);
 
    insertHeadList(&pList,5);   //表头插入元素12
    printList(pList);
 
    //insertLastList(&pList,10);  //表尾插入元素10
    printList(pList);
 
    clearList(pList);       //清空链表
    system("pause");
     
}


