#include "node.h"

void makeIp();
int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data);
int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data);
int stopNode(const struct _u_request *request, struct _u_response *response, void *user_data);
void *toAnalize(void *arg);
void funciondGabo(int key, char *image);
void confirmImageDone(int nodeId, int threadId);

int startNode() {

    makeIp();

    pthread_create(&t0, NULL, &toAnalize, (void*) (__intptr_t) 1);
    pthread_create(&t1, NULL, &toAnalize, (void*) (__intptr_t) 2);
    pthread_create(&t2, NULL, &toAnalize, (void*) (__intptr_t) 3);

    pthread_detach(t0);
    pthread_detach(t1);
    pthread_detach(t2);

    struct _u_instance instance;

    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        printf("Error ulfius_init_instance, abort\n");
        return 1;
    }

    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &allowCORS, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/Node/Analyze", NULL, 0, &receiveImage, NULL);
    ulfius_add_endpoint_by_val(&instance, "GET", "/Node/Stop", NULL, 0, &stopNode, NULL);

    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start node %d on port: %d\n",id,instance.port);

        while (1) {
            waitSemaphore(0);

            if (closeNode == 1) {
                sleep(1);
                break;
            }
        }
    } else {
        printf("Error starting node\n");
    }

    printf("Node ended\n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
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

int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data) {
    u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
    u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
    u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
    return U_CALLBACK_COMPLETE;
}

int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *jsonImage = ulfius_get_json_body_request(request, NULL);

    if (jsonImage != NULL) {
        printf("Image received\n");

        // Get id
        json_auto_t * idThread = NULL;
        idThread = json_object_get(jsonImage, "threadId");
        int tid = json_integer_value(idThread);

        // Get image content
        json_auto_t * imgB64 = NULL;
        imgB64 = json_object_get(jsonImage, "image");
        const char * img = json_string_value(imgB64);

        // Get token 
        json_auto_t * cypher = NULL;
        cypher = json_object_get(jsonImage, "key");

        switch (tid) {
        case 1:
            image0 = strdup(img);
            key0 = json_integer_value(cypher);
            break;
        case 2:
            image1 = strdup(img);
            key1 = json_integer_value(cypher);
            break;
        case 3:
            image2 = strdup(img);
            key2 = json_integer_value(cypher);
            break;
        }
        
        json_decref(jsonImage);
        json_decref(idThread);
        json_decref(imgB64);
        json_decref(cypher);

        postSemaphore(tid);

    } else {
        printf("Error in the image received\n");
    }

    ulfius_set_string_body_response(response, 200, "Ok");
    return U_CALLBACK_CONTINUE;
}

int stopNode(const struct _u_request *request, struct _u_response *response, void *user_data) {
    printf("Closing node\n");
    ulfius_set_string_body_response(response, 200, "Ok");
    closeNode = 1;
    postSemaphore(0);
    postSemaphore(1);
    postSemaphore(2);
    postSemaphore(3);
    return U_CALLBACK_CONTINUE;
}

void *toAnalize(void *arg) {
    int tid = (int) (__intptr_t) arg;

    int keyLocal = 0;
    char *imageLocal;

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

        confirmImageDone(id, tid);
    }
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