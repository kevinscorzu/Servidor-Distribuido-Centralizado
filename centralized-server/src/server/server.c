#include "server.h"

int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data);
int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data);
int receiveConfirmation(const struct _u_request *request, struct _u_response *response, void *user_data);
int stopServer(const struct _u_request *request, struct _u_response *response, void *user_data);

/**
 * Function that starts the server
 */
int startServer() {

    if (pthread_mutex_init(&lock, NULL) != 0) {
        writeToLog("Status: Failed to initialize mutex");
        return 1;
    }

    struct _u_instance instance;

    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        writeToLog("Status: Error ulfius_init_instance, abort");
        return 1;
    }

    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &allowCORS, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/Api/Analyze", NULL, 0, &receiveImage, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/Api/Confirm", NULL, 0, &receiveConfirmation, NULL);
    ulfius_add_endpoint_by_val(&instance, "GET", "/Api/Stop", NULL, 0, &stopServer, NULL);

    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start server on port: %d\n", instance.port);
        writeToLogInt("Status: Start server on port: ", instance.port);
        while (1) {
            waitSemaphore(0);

            if (closeServer == 1) {
                pthread_join(connectionThread, NULL);
                sleep(1);
                break;
            }

            printf("There are currently %d elements on the queue\n", imageList.count);
            postSemaphore(0);
            usleep(500000);
        }
    } else {
        writeToLog("Status: Error starting server");
    }

    printf("Server ended\n");
    writeToLog("Status: Server ended");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    pthread_mutex_destroy(&lock);

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
 * Function that receives an image from the client
 */
int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *jsonImage = ulfius_get_json_body_request(request, NULL);

    if (jsonImage != NULL) {
        queueAddItem(&imageList, jsonImage);
        postSemaphore(0);
        writeToLog("Status: Image received");
    } else {
        writeToLog("Status: Error in the image received");
    }

    ulfius_set_string_body_response(response, 200, "Ok");
    return U_CALLBACK_CONTINUE;
}

/**
 * Function that receives confirmation from a node
 */
int receiveConfirmation(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *jsonImage = ulfius_get_json_body_request(request, NULL);

    if (jsonImage != NULL) {
        json_t *idNodeJson = json_object_get(jsonImage, "node");
        int idNode = json_integer_value(idNodeJson);

        json_t *idThreadJson = json_object_get(jsonImage, "thread");
        int idThread = json_integer_value(idThreadJson);

        pthread_mutex_lock(&lock);

        updateNodeImages(idNode, idThread);

        pthread_mutex_unlock(&lock);

        json_decref(jsonImage);
        json_decref(idNodeJson);
        json_decref(idThreadJson);

        postSemaphore(1);
        writeToLog("Status: Confirmation received");
    } else {
        writeToLog("Status: Error in the image received");
    }

    ulfius_set_string_body_response(response, 200, "Ok");
    return U_CALLBACK_CONTINUE;
}

/**
 * Function that stops the server
 */
int stopServer(const struct _u_request *request, struct _u_response *response, void *user_data) {
    writeToLog("Status: Closing server");
    ulfius_set_string_body_response(response, 200, "Ok");
    closeServer = 1;
    postSemaphore(0);
    postSemaphore(0);
    return U_CALLBACK_CONTINUE;
}