#include <stdio.h>
#include <stdlib.h>
#include "utils/semaphore.h"
#include "utils/config.h"
#include "node/node.h"
#include "node/image.h"

/**
 * Main function of the processor node
 */
int main() {
    readConfigFile();
    createSemaphore();
    closeNode = 0;
    initializeImageHandler();
    startNode();
    closeSemaphore();

    return 0;
}