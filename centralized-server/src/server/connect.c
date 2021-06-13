#include "connect.h"

void makeIps();
void *sendImages(void *arg);
char* getIp();

int startNodeConnectionHandler() {

    currentNode1Images = 0;
    currentNode2Images = 0;
    makeIps();

    pthread_t connectionThread;
    pthread_create(&connectionThread, NULL, sendImages, NULL);
    pthread_detach(connectionThread);

    writeToLog("Status: Successfully started the connection handler thread");

    return 0;
}

int updateNodeImages(int id) {
    switch (id) {
    case 0:
        if (currentNode1Images == 0) {
            return 0;
        }
        currentNode1Images -= 1;
        break;
    case 1:
        if (currentNode2Images == 0) {
            return 0;
        }
        currentNode2Images -= 1;
        break;
    }

    return 0;
}

void makeIps() {

    char* firstPart = "http://";
    char* secondPart = "/Api/Receive";

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
    CURL *curl;
    CURLcode res;
    char *ip;
    json_t *image;

    while (1) {
        waitSemaphore(1);
        waitSemaphore(0);

        if (closeServer == 1) {
            break;
        }
        
        image = (json_t*) queueGetFirstItem(&imageList);
        ip = getIp();

        curl_global_init(CURL_GLOBAL_ALL);
        
        curl = curl_easy_init();
        if (curl) {

            curl_easy_setopt(curl, CURLOPT_URL, ip);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, image);
        
            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
    
    free(node1Link);
    free(node2Link);
}

char* getIp() {
    if (currentNode1Images != 3) {
        currentNode1Images += 1;
        return node1Link;
    }
    else {
        currentNode2Images += 1;
        return node2Link;
    }
}