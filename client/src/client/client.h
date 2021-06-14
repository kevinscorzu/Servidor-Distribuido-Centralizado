#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jansson.h"
#include "ulfius.h"

struct _u_request requestConnection;
struct _u_response responseConnection;

json_t *jsonImage;

char *serverIp;

char *serverImageLink;

char *serverCloseLink;

void makeIp();
int stopServer();
int sendImageToServer();

#endif