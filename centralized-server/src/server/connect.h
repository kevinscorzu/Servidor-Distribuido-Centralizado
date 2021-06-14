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

struct _u_request request;
struct _u_response response;

char *node1Ip;
char *node2Ip;

char *node1Link;
char *node2Link;

int currentNode1Images[3];
int currentNode1ImagesCount;

int currentNode2Images[3];
int currentNode2ImagesCount;

int lastNodeSent;

int startNodeConnectionHandler();
int updateNodeImages(int id, int thread);

#endif