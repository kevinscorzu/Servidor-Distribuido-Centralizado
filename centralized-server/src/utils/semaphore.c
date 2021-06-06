#include "semaphore.h"

int createSemaphore() {

    sem = sem_open("sem", O_CREAT, 0644, 0);

    if (sem == SEM_FAILED) {
        writeToLog("Status: Error while creating the semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int closeSemaphore() {

    if (sem_close(sem) != 0) {
        writeToLog("Status: Error while closing the semaphore");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("sem") < 0) {
        writeToLog("Status: Error while unlinking the semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int waitSemaphore() {

    if (sem_wait(sem) < 0) {
        writeToLog("Status: Error while waiting for the semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int postSemaphore() {
    
    if (sem_post(sem) < 0) {
        writeToLog("Status: Error while posting the semaphore");
        exit(EXIT_FAILURE);
    }

    return 0;
}