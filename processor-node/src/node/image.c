#include "image.h"

void makeIp();
void *toAnalize(void *arg);
void confirmImageDone(int nodeId, int threadId);

void initializeImageHandler() {

    makeIp();

    pthread_create(&t0, NULL, toAnalize, (void*) (__intptr_t) 1);
    pthread_create(&t1, NULL, toAnalize, (void*) (__intptr_t) 2);
    pthread_create(&t2, NULL, toAnalize, (void*) (__intptr_t) 3);

}

void makeIp() {

    char *firstPart = "http://";
    char *secondPart = "/Api/Confirm";

    serverLink = malloc(sizeof(firstPart) + sizeof(serverIp) + sizeof(secondPart) + 1);

    strcpy(serverLink, firstPart);
    strcat(serverLink, serverIp);
    strcat(serverLink, secondPart);

    free(serverIp);

}

void *toAnalize(void *arg) {
    int tid = (int) (__intptr_t) arg;

    int keyLocal = 0;
    char* imageLocal;

    while(1){
        waitSemaphore(tid);
        if (closeNode == 1) {
            break;
        }

        switch (tid) {
        case 1:
            imageLocal = image0;
            keyLocal = key0;
            break;
        case 2:
            imageLocal = image1;
            keyLocal = key1;
            break;
        case 3:
            imageLocal = image2;
            keyLocal = key2;
            break;
        }

        //funciondGabo(keyLocal,imageLocal);

        free(imageLocal);
        confirmImageDone(id, tid);
    }

    pthread_exit(NULL);
}


void confirmImageDone(int nodeId, int threadId) {
    struct _u_response response;
    struct _u_request request;

    json_t *confirmJson = json_object();
    int res;

    json_object_set_new(confirmJson, "node", json_integer(nodeId));
    json_object_set_new(confirmJson, "thread", json_integer(threadId));

    ulfius_init_request(&request);

    ulfius_set_request_properties(&request,
                                  U_OPT_HTTP_VERB, "POST",
                                  U_OPT_HTTP_URL, serverLink,
                                  U_OPT_JSON_BODY, confirmJson,
                                  U_OPT_NONE);

    ulfius_init_response(&response);

    res = ulfius_send_http_request(&request, &response);
    if (res == U_OK) {
        printf("Sent confirmation to server\n");
    }
    else {
        printf("Failed to send confirmation to server\n");
    }

    ulfius_clean_response(&response);
    ulfius_clean_request(&request);

    json_decref(confirmJson);
}