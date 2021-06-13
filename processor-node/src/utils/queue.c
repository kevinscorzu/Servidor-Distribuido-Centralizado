#include "queue.h"

void queueInit(queue* list) {
    list->head = NULL;
    list->count = 0; 
    return;
};

void queueAddItem(queue* list, queueItem item) {
    queueNode* newNode = (queueNode*) malloc(sizeof(queueNode));
    if (newNode == NULL) {
        writeToLog("Status: Error while allocating memory");
        exit(EXIT_FAILURE);
    }
    
    newNode->item = item;
    newNode->next = NULL;

    if (list->head == NULL){
        list->head = newNode;

    } else {
        queueNode* tmp = list->head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = newNode;
    }

    list->count++;
    return;
}

queueItem queueGetFirstItem(queue* list) {
    if(list->head == NULL){
        writeToLog("Status: Error the list is empty");
        exit(EXIT_FAILURE);
    }

    queueNode* tmp = list->head;
    queueItem item = tmp->item;

    list->head = list->head->next;

    free(tmp);
    list->count--;
    return item;
}