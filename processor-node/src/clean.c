#include <stdio.h>
#include <stdlib.h>
#include "utils/semaphore.h"

/**
 * Main function of the clean file
 */
int main() {
    findSemaphore();
    closeSemaphore();
    printf("Successfully closed the semaphore\n");

    return 0;
}