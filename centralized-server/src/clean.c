#include <stdio.h>
#include <stdlib.h>
#include "utils/semaphore.h"

int main() {
    findSemaphores();
    closeSemaphores();
    printf("Successfully closed the semaphores\n");

    return 0;
}