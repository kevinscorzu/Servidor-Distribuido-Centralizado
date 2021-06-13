#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include "log.h"

sem_t *semImageQueue;
sem_t *semImageSend;

int createSemaphores();
int closeSemaphores();
int waitSemaphore(int semaphore);
int postSemaphore(int semaphore);
int findSemaphores();

#endif