#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t *semThreadMain;
sem_t *semThread0;
sem_t *semThread1;
sem_t *semThread2;

int createSemaphore();
int closeSemaphore();
int waitSemaphore(int semaphore);
int postSemaphore(int semaphore);
int findSemaphore();

#endif