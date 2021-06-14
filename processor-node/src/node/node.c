#include "node.h"

void makeIp();
int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data);
int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data);
int stopNode(const struct _u_request *request, struct _u_response *response, void *user_data);
void *toAnalize(void *arg);
void funciondGabo(int key, char *image);
void confirmImageDone(int nodeId, int threadId);

/**
 * Function that starts the node
 */
int startNode() {

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
                pthread_join(t0, NULL);
                pthread_join(t1, NULL);
                pthread_join(t2, NULL);
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

/**
 * Function that handles the cors of the server
 */
int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data) {
    u_map_put(response->map_header, "Access-Control-Allow-Origin", "*");
    u_map_put(response->map_header, "Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    u_map_put(response->map_header, "Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, Bearer, Authorization");
    u_map_put(response->map_header, "Access-Control-Max-Age", "1800");
    return U_CALLBACK_COMPLETE;
}

/**
 * Function that receives an image from the server
 */
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

        // Get image name
        json_auto_t * imgName = NULL;
        imgName = json_object_get(jsonImage, "name");
        const char * imgN = json_string_value(imgName);

        // Get key 
        json_auto_t * cypher = NULL;
        cypher = json_object_get(jsonImage, "key");

        switch (tid) {
        case 1:
            image0 = strdup(img);
            imageName0 = strdup(imgN);
            key0 = json_integer_value(cypher);
            break;
        case 2:
            image1 = strdup(img);
            imageName1 = strdup(imgN);
            key1 = json_integer_value(cypher);
            break;
        case 3:
            image2 = strdup(img);
            imageName2 = strdup(imgN);
            key2 = json_integer_value(cypher);
            break;
        }
        
        json_decref(jsonImage);
        json_decref(idThread);
        json_decref(imgB64);
        json_decref(imgName);
        json_decref(cypher);

        postSemaphore(tid);

    } else {
        printf("Error in the image received\n");
    }

    ulfius_set_string_body_response(response, 200, "Ok");
    return U_CALLBACK_CONTINUE;
}

/**
 * Function that stops the node
 */
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