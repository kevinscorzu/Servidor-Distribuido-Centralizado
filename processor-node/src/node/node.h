#ifndef __NODE_H
#define __NODE_H

#include <ulfius.h>
#include <jansson.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../utils/log.h"
#include "../utils/queue.h"
#include "../utils/semaphore.h"

int port;
int id;
int closeNode;

int startNode();

#endif