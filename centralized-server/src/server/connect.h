#ifndef __CONNECT_H
#define __CONNECT_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <jansson.h>
#include "../utils/log.h"
#include "../utils/semaphore.h"
#include "../utils/queue.h"
#include "server.h"

pthread_t connectionThread;

struct _u_request requestConnection;
struct _u_response responseConnection;

char *node1Ip;
char *node2Ip;

char *node1ImageLink;
char *node2ImageLink;

char *node1CloseLink;
char *node2CloseLink;

int currentNode1Images[3];
int currentNode1ImagesCount;

int currentNode2Images[3];
int currentNode2ImagesCount;

int lastNodeSent;

int startNodeConnectionHandler();
int updateNodeImages(int id, int thread);

#endif