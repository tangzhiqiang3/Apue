#include <string.h>
#include <malloc.h>
#include <iostream>

using namespace std;

typedef struct linkList{
    int data;
    struct linkList* nextNode;
}linkList_t;

typedef struct linkContorl{
    linkList_t* head;
    linkList_t* tail;
}linkContorl_t;

void linkList_Init(linkContorl_t** lCtrl){
    *lCtrl = (linkContorl_t*)malloc(sizeof(linkContorl_t));
    memset(*lCtrl, 0, sizeof(linkContorl_t));
}

bool linkList_addNode(linkContorl_t* lCtrl, int nodeData){
    linkList_t* newNode = (linkList_t*)malloc(sizeof(linkList_t));
    if(!newNode){
        return false;
    }

    newNode->data = nodeData;   
    newNode->nextNode = NULL;   

    if((lCtrl->head == NULL) && (lCtrl->tail == NULL)){
        lCtrl->head = newNode;
        lCtrl->tail = newNode;          
    }
    else{
        lCtrl->tail->nextNode = newNode;
        lCtrl->tail = newNode;
    }

    return true;
}

bool linkList_removeNode(linkContorl_t* lCtrl){
    linkList_t* node = lCtrl->head;
    if(node != NULL){
        lCtrl->head = node->nextNode;
        if(node == lCtrl->tail){
            lCtrl->tail = NULL;
        }
        if(node){
            free(node);
        }
        return true;    
    }
    else{
        return false;
    }
}

void linkList_Deinit(linkContorl_t* lCtrl){
    linkList_t* tmpP = lCtrl->head;
    linkList_t* tmpQ;
    while(tmpP != NULL){
        tmpQ = tmpP;
        tmpP = tmpP->nextNode;      
        free(tmpQ);
    }
    if(lCtrl){
        free(lCtrl);
        lCtrl = NULL;
    }
}

void printLinkList(linkContorl_t* lCtrl){
    linkList_t* tmpP = lCtrl->head;
    while(tmpP != NULL){
        cout << tmpP->data << " ";
        tmpP = tmpP->nextNode;      
    }
    cout << endl;
}

int main()
{
    linkContorl_t* lCtrl;
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

    linkList_addNode(lCtrl, 30);
    printLinkList(lCtrl);   
}


