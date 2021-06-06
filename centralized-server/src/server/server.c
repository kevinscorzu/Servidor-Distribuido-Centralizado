#include "server.h"

int allowCORS(const struct _u_request *request, struct _u_response *response, void *user_data);
int receiveImage(const struct _u_request *request, struct _u_response *response, void *user_data);
int stopServer(const struct _u_request *request, struct _u_response *response, void *user_data);

int startServer() {
    struct _u_instance instance;

    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        writeToLog("Status: Error ulfius_init_instance, abort");
        return 1;
    }

    ulfius_add_endpoint_by_val(&instance, "OPTIONS", NULL, "*", 0, &allowCORS, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/Api/Analyze", NULL, 0, &receiveImage, NULL);
    ulfius_add_endpoint_by_val(&instance, "GET", "/Api/Stop", NULL, 0, &stopServer, NULL);

    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start server on port: %d\n", instance.port);
        writeToLogInt("Status: Start server on port: ", instance.port);
        while (1) {
            waitSemaphore();

            if (closeServer == 1) {
                sleep(1);
                break;
            }

            printf("There are currently %d elements on the queue\n", imageList.count);
            postSemaphore();
            usleep(500000);
        }
    } else {
        writeToLog("Status: Error starting server");
    }

    printf("Server ended\n");
    writeToLog("Status: Server ended");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
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
    char *jsonString = json_dumps(jsonImage, JSON_ENCODE_ANY);

    if (jsonImage != NULL) {
        queueAddItem(&imageList, jsonString);
        postSemaphore();
        writeToLog("Status: Image received");
    } else {
        writeToLog("Status: Error in the image received");
    }

    free(jsonImage);
    ulfius_set_string_body_response(response, 200, "Ok");
    return U_CALLBACK_CONTINUE;
}

int stopServer(const struct _u_request *request, struct _u_response *response, void *user_data) {
    writeToLog("Status: Closing server");
    ulfius_set_string_body_response(response, 200, "Ok");
    closeServer = 1;
    postSemaphore();
    postSemaphore();
    return U_CALLBACK_CONTINUE;
}