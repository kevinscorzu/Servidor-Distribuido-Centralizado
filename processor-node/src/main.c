#include <stdio.h>
#include <stdlib.h>
#include "utils/log.h"
#include "utils/config.h"
#include "utils/queue.h"
#include "utils/semaphore.h"

int main() {
    readConfigFile();
    queueInit(&imageList);
    createSemaphore();
    closeNode = 0;
    startNode();
    closeSemaphore();

    return 0;
}