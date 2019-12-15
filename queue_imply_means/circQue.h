#include <stdlib.h>
#include <stdbool.h>

void circQue_Init(circQue_t *queue);
void circQue_Deinit(circQue_t *queue);
bool isQueEmpty(circQue_t *queue);
bool isQueFull(circQue_t *queue);
bool circQue_add(circQue_t *queue, char newData);
char circQue_remove(circQue_t *queue);
void printQueueData(circQue_t *queue);
