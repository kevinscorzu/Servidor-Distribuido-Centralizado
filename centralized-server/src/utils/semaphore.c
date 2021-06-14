#include "semaphore.h"

/**
 * Function that creates the semaphores for the centralized server
 */
int createSemaphores() {

    semImageQueue = sem_open("semImageQueue", O_CREAT, 0644, 0);

    if (semImageQueue == SEM_FAILED) {
        writeToLog("Status: Error while creating the Image Queue semaphore");
        exit(EXIT_FAILURE);
    }

    semImageSend = sem_open("semImageSend", O_CREAT, 0644, 6);

    if (semImageSend == SEM_FAILED) {
        writeToLog("Status: Error while creating the Image Send semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/**
 * Function that closes the semaphores of the centralized server
 */
int closeSemaphores() {

    if (sem_close(semImageQueue) != 0) {
        writeToLog("Status: Error while closing the Image Queue semaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semImageQueue") < 0) {
        writeToLog("Status: Error while unlinking the Image Queue semaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semImageSend) != 0) {
        writeToLog("Status: Error while closing the Image Send semaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semImageSend") < 0) {
        writeToLog("Status: Error while unlinking the Image Send semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/**
 * Function that waits a semaphore
 * Receives an integer indicating the semaphore that will be used
 */
int waitSemaphore(int semaphore) {

    switch (semaphore) {
    case 0:
        if (sem_wait(semImageQueue) < 0) {
            writeToLog("Status: Error while waiting for the Image Queue semaphore");
            exit(EXIT_FAILURE);
        }
        break;
    case 1:
        if (sem_wait(semImageSend) < 0) {
            writeToLog("Status: Error while waiting for the Image Send semaphore");
            exit(EXIT_FAILURE);
        }
        break;
    }

    return 0;
}

/**
 * Function that posts a semaphore
 * Receives an integer indicating the semaphore that will be used
 */
int postSemaphore(int semaphore) {
    
    switch (semaphore) {
    case 0:
        if (sem_post(semImageQueue) < 0) {
            writeToLog("Status: Error while posting the Image Queue semaphore");
            exit(EXIT_FAILURE);
        }
        break;
    case 1:
        if (sem_post(semImageSend) < 0) {
            writeToLog("Status: Error while posting the Image Send semaphore");
            exit(EXIT_FAILURE);
        }
        break;
    }
    
    return 0;
}

/**
 * Function that finds semaphores for the centralized server
 */
int findSemaphores() {
    
    semImageQueue = sem_open("semImageQueue", 0);

    if (semImageQueue == SEM_FAILED) {
        writeToLog("Status: Error while finding the Image Queue semaphore");
        exit(EXIT_FAILURE);
    }

    semImageSend = sem_open("semImageSend", 0);

    if (semImageSend == SEM_FAILED) {
        writeToLog("Status: Error while finding the Image Send semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}