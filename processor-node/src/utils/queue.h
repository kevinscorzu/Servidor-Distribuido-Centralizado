#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#include "log.h"

typedef void* queueItem;

typedef struct queueNode{
    queueItem item;
    struct queueNode* next;
}queueNode;

typedef struct queue{
    queueNode* head;
    int count;
}queue;

queue imageList;

void queueInit(queue* list);
void queueAddItem(queue* list, queueItem item);
queueItem queueGetFirstItem(queue* list);

#endif