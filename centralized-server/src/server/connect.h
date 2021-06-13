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

char *node1Ip;
char *node2Ip;
char *node1Link;
char *node2Link;
int currentNode1Images;
int currentNode2Images;

int startNodeConnectionHandler();
int updateNodeImages(int id);

#endif