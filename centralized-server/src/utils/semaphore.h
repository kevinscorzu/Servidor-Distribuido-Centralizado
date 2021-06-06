#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include "log.h"

sem_t* sem;

int createSemaphore();
int closeSemaphore();
int waitSemaphore();
int postSemaphore();

#endif