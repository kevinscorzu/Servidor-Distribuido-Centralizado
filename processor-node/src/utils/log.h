#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int writeToLog(char *message);
int writeToLogInt(char *message, char *auxMessage, int number, int portNumber);

#endif