#ifndef __NODE_H
#define __NODE_H

#include <ulfius.h>
#include <jansson.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../utils/semaphore.h"
#include "image.h"

char *imageName0;
char *image0;
int key0;

char *imageName1;
char *image1;
int key1;

char *imageName2;
char *image2;     
int key2;

int port;
int id;
int closeNode;

int startNode();

#endif