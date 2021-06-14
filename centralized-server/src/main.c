#include <stdio.h>
#include <stdlib.h>
#include "utils/log.h"
#include "utils/config.h"
#include "utils/queue.h"
#include "utils/semaphore.h"
#include "server/server.h"
#include "server/connect.h"

int main() {
    //readConfigFile();
    //queueInit(&imageList);
    //createSemaphores();
    //closeServer = 0;
    startNodeConnectionHandler();
    startServer();
    closeSemaphores();    

    return 0;
}