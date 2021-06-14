#ifndef __NODE_H
#define __NODE_H

#define SERVER_URL_PREFIX "http://localhost:2020/Api/Stop"

#include <ulfius.h>
#include <jansson.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../utils/semaphore.h"

pthread_t t0;
pthread_t t1;
pthread_t t2;

char *serverIp;

char *serverLink;

char *image0;
int key0;

char *image1;
int key1;

char *image2;     
int key2;

int port;
int id;
int closeNode;

int startNode();

#endif