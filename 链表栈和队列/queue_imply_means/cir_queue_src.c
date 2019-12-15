#include <string.h>
#include <malloc.h>
#include <iostream>

#define MAX_NODE_NUM 10

using namespace std;

typedef struct circQueue{
    int front;
    int rear;
    char data[MAX_NODE_NUM];
}circQue_t; 

void circQue_Init(circQue_t* queue){        
    queue->front = 0;
    queue->rear = 0;
    memset(queue->data, 0, MAX_NODE_NUM);
}

void circQue_Deinit(circQue_t* queue){
    if(queue){
        cout << "will free queue,front is:" << queue->front <<",rear is:" << queue->rear << endl;
        free(queue);
    }
}

bool isQueEmpty(circQue_t* queue){
    return (queue->front == queue->rear);//when front equal rear means circular queque is empty
}

bool isQueFull(circQue_t* queue){
    return (((queue->rear + 1) % MAX_NODE_NUM == queue->front));//get the value of rear plus 1, then mod capacity of circular queue, queue is full if the result of mod is equal front  
}

bool circQue_add(circQue_t* queue, char newData){
    if(isQueFull(queue)){
        cout << "queue is full, add node failed!" << endl;  
        return false;   
    }
    queue->data[queue->rear] = newData;
    queue->rear = (queue->rear+1)%MAX_NODE_NUM;
    cout << "front is:" << queue->front << ",rear is:" << queue->rear << endl;
    return true;
}

char circQue_remove(circQue_t* queue){
    if(isQueEmpty(queue)){
        cout << "queue is empty, remove node failed!" << endl;  
        return 'x';         
    }
    char nodeValue = queue->data[queue->front];
    queue->front = (queue->front+1)%MAX_NODE_NUM;
    return  nodeValue;
}

void printQueueData(circQue_t* queue){
    for(int i = queue->front; i != queue->rear; i=(i+1)%MAX_NODE_NUM){
        cout << queue->data[i] << " ";
    }
    cout << endl;
}

int main(){
    circQue_t* que = (circQue_t*)malloc(sizeof(circQue_t));
    if(!que){
        cout << "malloc for circular queque failed!" << endl;
        return -1;  
    }   

    circQue_Init(que);

    circQue_add(que, 'a');
    circQue_add(que, 'b');
    circQue_add(que, 'c');
    printQueueData(que);
    circQue_remove( que);
    printQueueData(que);
}

