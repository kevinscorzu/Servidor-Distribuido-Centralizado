#ifndef __SERVER_H
#define __SERVER_H

#include <ulfius.h>
#include <jansson.h>
#include <unistd.h>
#include "../utils/log.h"
#include "../utils/queue.h"
#include "../utils/semaphore.h"
#include "connect.h"

int port;
int closeServer;

int startServer();

#endif