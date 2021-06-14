#ifndef __IMAGE_H
#define __IMAGE_H

#include <jansson.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "node.h"

pthread_t t0;
pthread_t t1;
pthread_t t2;

char *serverIp;

char *serverLink;

void initializeImageHandler();

#endif