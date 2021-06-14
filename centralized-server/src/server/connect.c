#include "connect.h"

void makeIps();
void *sendImages(void *arg);
int getNode();
char* getIp(int node);
int getThread(int node);

int startNodeConnectionHandler() {

    for (int i = 0; i < 3; i++) {
        currentNode1Images[i] = 0;
        currentNode2Images[i] = 0;
    }

    currentNode1ImagesCount = 0;
    currentNode2ImagesCount = 0;
    lastNodeSent = 1;

    ulfius_init_request(&request);
    makeIps();

    pthread_t connectionThread;
    pthread_create(&connectionThread, NULL, sendImages, NULL);
    pthread_detach(connectionThread);

    writeToLog("Status: Successfully started the connection handler thread");

    return 0;
}

int updateNodeImages(int id, int thread) {

    switch (id) {
    case 0:
        currentNode1ImagesCount--;
        switch (thread) {
        case 1:
            currentNode1Images[0] = 0;
            break;
        case 2:
            currentNode1Images[1] = 0;
            break;
        case 3:
            currentNode1Images[2] = 0;
            break;
        }
        break;
    case 1:
        currentNode2ImagesCount--;
        switch (thread) {
        case 1:
            currentNode2Images[0] = 0;
            break;
        case 2:
            currentNode2Images[1] = 0;
            break;
        case 3:
            currentNode2Images[2] = 0;
            break;
        }
        break;
    }

    return 0;
}

void makeIps() {

    char* firstPart = "http://";
    char* secondPart = "/Node/Analyze";

    node1Link = malloc(sizeof(firstPart) + sizeof(node1Ip) + sizeof(secondPart) + 1);
    node2Link = malloc(sizeof(firstPart) + sizeof(node2Ip) + sizeof(secondPart) + 1);

    strcpy(node1Link, firstPart);
    strcat(node1Link, node1Ip);
    strcat(node1Link, secondPart);

    strcpy(node2Link, firstPart);
    strcat(node2Link, node2Ip);
    strcat(node2Link, secondPart);

    free(node1Ip);
    free(node2Ip);

}

void *sendImages(void *arg) {
    
    int node;
    char *ip;
    int thread;
    int res;

    json_t *image;

    while (1) {
        waitSemaphore(1);
        waitSemaphore(0);

        if (closeServer == 1) {
            break;
        }
        
        image = (json_t*) queueGetFirstItem(&imageList);
        node = getNode();
        ip = getIp(node);
        thread = getThread(node);

        json_object_set_new(image, "threadId", json_integer(thread));
        
        ulfius_set_request_properties(&request,
                                      U_OPT_HTTP_VERB, "POST",
                                      U_OPT_HTTP_URL, ip,
                                      U_OPT_TIMEOUT, 20,
                                      U_OPT_JSON_BODY, image,
                                      U_OPT_NONE);

        ulfius_init_response(&response);

        res = ulfius_send_http_request(&request, &response);
        if (res == U_OK) {
            printf("Image sent to node %d\n", node);
            writeToLogInt("Status: Image sent to node ", node);
        }
        else {
            printf("Failed to send image to node %d\n", node);
            writeToLogInt("Status: Failed to send image to node ", node);
        }

        ulfius_clean_response(&response);
        ulfius_clean_request(&request);

        json_decref(image);
    }
    
    free(node1Link);
    free(node2Link);
}

int getNode() {

    if (currentNode1ImagesCount == 3) {
        currentNode2ImagesCount++;
        lastNodeSent = 2;
        return 2;
    }
    else if (currentNode2ImagesCount == 3) {
        currentNode1ImagesCount++;
        lastNodeSent = 1;
        return 1;
    }
    else {
        switch (lastNodeSent) {
        case 1:
            currentNode2ImagesCount++;
            lastNodeSent = 2;
            return 2;
        case 2:
            currentNode1ImagesCount++;
            lastNodeSent = 1;
            return 1;
        }
    }

}

char* getIp(int node) {
    switch (node) {
    case 1:
        return node1Link;
    case 2:
        return node2Link;
    }
}

int getThread(int node) {
    switch (node) {
    case 1:
        for (int i = 0; i < 3; i++) {
            if (currentNode1Images[i] == 0) {
                currentNode1Images[i] = 1;
                return i;
            }
        }
        break;
    case 2:
        for (int i = 0; i < 3; i++) {
            if (currentNode2Images[i] == 0) {
                currentNode2Images[i] = 1;
                return i;
            }
        }
        break;
    }
}