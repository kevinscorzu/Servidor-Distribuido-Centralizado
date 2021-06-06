#include <stdio.h>
#include <stdlib.h>
#include "utils/semaphore.h"

int main() {
    findSemaphore();
    closeSemaphore();
    printf("Successfully closed the semaphore");

    return 0;
}