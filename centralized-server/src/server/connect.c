#include "connect.h"

void makeIps();
void *sendImages(void *arg);
int getNode();
char* getIp(int node);
int getThread(int node);
int stopNode(char* ip, int node);

/**
 * Function that starts the connection thread
 */
int startNodeConnectionHandler() {

    for (int i = 0; i < 3; i++) {
        currentNode1Images[i] = 0;
        currentNode2Images[i] = 0;
    }

    currentNode1ImagesCount = 0;
    currentNode2ImagesCount = 0;
    lastNodeSent = 1;

    makeIps();

    pthread_create(&connectionThread, NULL, sendImages, NULL);

    writeToLog("Status: Successfully started the connection handler thread");

    return 0;
}

/**
 * Function that updates the nodes current information
 * Receives the id of the node and the thread id of the node
 */
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

/**
 * Function that makes the ips to connect to the nodes
 */
void makeIps() {

    char* firstPart = "http://";
    char* secondPartImage = "/Node/Analyze";
    char* secondPartStop = "/Node/Stop";

    node1ImageLink = malloc(sizeof(firstPart) + sizeof(node1Ip) + sizeof(secondPartImage) + 1);
    node2ImageLink = malloc(sizeof(firstPart) + sizeof(node2Ip) + sizeof(secondPartImage) + 1);

    node1CloseLink = malloc(sizeof(firstPart) + sizeof(node1Ip) + sizeof(secondPartStop) + 1);
    node2CloseLink = malloc(sizeof(firstPart) + sizeof(node2Ip) + sizeof(secondPartStop) + 1);

    strcpy(node1ImageLink, firstPart);
    strcat(node1ImageLink, node1Ip);
    strcat(node1ImageLink, secondPartImage);

    strcpy(node2ImageLink, firstPart);
    strcat(node2ImageLink, node2Ip);
    strcat(node2ImageLink, secondPartImage);

    strcpy(node1CloseLink, firstPart);
    strcat(node1CloseLink, node1Ip);
    strcat(node1CloseLink, secondPartStop);

    strcpy(node2CloseLink, firstPart);
    strcat(node2CloseLink, node2Ip);
    strcat(node2CloseLink, secondPartStop);

    free(node1Ip);
    free(node2Ip);

}

/**
 * Function that sends images to the nodes
 */
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
            stopNode(node1CloseLink, 0);
            stopNode(node2CloseLink, 1);

            break;
        }
        
        image = (json_t*) queueGetFirstItem(&imageList);

        pthread_mutex_lock(&lock);

        node = getNode();
        ip = getIp(node);
        thread = getThread(node);

        pthread_mutex_unlock(&lock);

        json_object_set_new(image, "threadId", json_integer(thread));

        ulfius_init_request(&requestConnection);
        
        ulfius_set_request_properties(&requestConnection,
                                      U_OPT_HTTP_VERB, "POST",
                                      U_OPT_HTTP_URL, ip,
                                      U_OPT_JSON_BODY, image,
                                      U_OPT_NONE);

        ulfius_init_response(&responseConnection);

        res = ulfius_send_http_request(&requestConnection, &responseConnection);
        if (res == U_OK) {
            printf("Image sent to node %d\n", node);
            writeToLogInt("Status: Image sent to node ", node);
        }
        else {
            printf("Failed to send image to node %d\n", node);
            writeToLogInt("Status: Failed to send image to node ", node);
        }

        ulfius_clean_response(&responseConnection);
        ulfius_clean_request(&requestConnection);

        json_decref(image);
    }
    
    free(node1ImageLink);
    free(node2ImageLink);

    free(node1CloseLink);
    free(node2CloseLink);

    pthread_exit(NULL);
}

/**
 * Function that returns the node that will receive the image
 */
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

/**
 * Function that returns the ip of the node that will be receive the image
 * Receives the id of the node
 */
char* getIp(int node) {
    switch (node) {
    case 1:
        return node1ImageLink;
    case 2:
        return node2ImageLink;
    }
}

/**
 * Function that returns the thread that will handle the image
 * Receives the id of the node
 */
int getThread(int node) {
    switch (node) {
    case 1:
        for (int i = 0; i < 3; i++) {
            if (currentNode1Images[i] == 0) {
                currentNode1Images[i] = 1;
                i++;
                return i;
            }
        }
        break;
    case 2:
        for (int i = 0; i < 3; i++) {
            if (currentNode2Images[i] == 0) {
                currentNode2Images[i] = 1;
                i++;
                return i;
            }
        }
        break;
    }
}

/**
 * Function that stops the nodes
 * Receives the ip of the node to be stopped and the integer of the node
 */
int stopNode(char* ip, int node) {

    int res;

    ulfius_init_request(&requestConnection);
        
    ulfius_set_request_properties(&requestConnection,
                                    U_OPT_HTTP_VERB, "GET",
                                    U_OPT_HTTP_URL, ip,
                                    U_OPT_NONE);

    ulfius_init_response(&responseConnection);

    res = ulfius_send_http_request(&requestConnection, &responseConnection);
    if (res == U_OK) {
        printf("Successfully stopped node %d\n", node);
        writeToLogInt("Status: Successfully stopped node ", node);
    }
    else {
        printf("Failed to stop node %d\n", node);
        writeToLogInt("Status: Failed to stop node ", node);
    }

    ulfius_clean_response(&responseConnection);
    ulfius_clean_request(&requestConnection);

    return 0;
}