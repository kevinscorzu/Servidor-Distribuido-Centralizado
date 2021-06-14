#include "semaphore.h"

/**
 * Function that creates the semaphores for the centralized server
 */
int createSemaphore() {

    semThreadMain = sem_open("semThreadMain", O_CREAT, 0644, 0);

    if (semThreadMain == SEM_FAILED) {
        printf("Error while creating the main thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread0 = sem_open("semThread0", O_CREAT, 0644, 0);

    if (semThread0 == SEM_FAILED) {
        printf("Error while creating the zero thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread1 = sem_open("semThread1", O_CREAT, 0644, 0);

    if (semThread1 == SEM_FAILED) {
        printf("Error while creating the first thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread2 = sem_open("semThread2", O_CREAT, 0644, 0);

    if (semThread2 == SEM_FAILED) {
        printf("Error while creating the second thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/**
 * Function that closes the semaphores of the centralized server
 */
int closeSemaphore() {

    if (sem_close(semThreadMain) != 0) {
        printf("Error while closing the main thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semThreadMain") < 0) {
        printf("Error while unlinking the main thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semThread0) != 0) {
        printf("Error while closing the zero thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semThread0") < 0) {
        printf("Error while unlinking the zero thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semThread1) != 0) {
        printf("Error while closing the first thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semThread1") < 0) {
        printf("Error while unlinking the first thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semThread2) != 0) {
        printf("Error while closing the second thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    if (sem_unlink("semThread2") < 0) {
        printf("Error while unlinking the second thread semaphore\n");
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
        if (sem_wait(semThreadMain) < 0) {
            printf("Error while waiting for the main thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 1:
        if (sem_wait(semThread0) < 0) {
            printf("Error while waiting for the zero thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 2:
        if (sem_wait(semThread1) < 0) {
            printf("Error while waiting for the first thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 3:
        if (sem_wait(semThread2) < 0) {
            printf("Error while waiting for the second thread semaphore\n");
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
        if (sem_post(semThreadMain) < 0) {
            printf("Error while posting the main thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 1:
        if (sem_post(semThread0) < 0) {
            printf("Error while posting the zero thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 2:
        if (sem_post(semThread1) < 0) {
            printf("Error while posting the first thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    case 3:
        if (sem_post(semThread2) < 0) {
            printf("Error while posting the second thread semaphore\n");
            exit(EXIT_FAILURE);
        }
        break;
    }

    return 0;
}

/**
 * Function that finds semaphores for the centralized server
 */
int findSemaphore() {
    
    semThreadMain = sem_open("semThreadMain", 0);

    if (semThreadMain == SEM_FAILED) {
        printf("Error while finding the main thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread0 = sem_open("semThread0", 0);

    if (semThread0 == SEM_FAILED) {
        printf("Error while finding the zero thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread1 = sem_open("semThread1", 0);

    if (semThread1 == SEM_FAILED) {
        printf("Error while finding the first thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    semThread2 = sem_open("semThread2", 0);

    if (semThread2 == SEM_FAILED) {
        printf("Error while finding the second thread semaphore\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}