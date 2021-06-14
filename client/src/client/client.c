#include "client.h"

void makeIp() {

    char *firstPart = "http://";
    char *secondPartImage = "/Api/Analyze";
    char* secondPartStop = "/Api/Stop";

    serverImageLink = malloc(sizeof(firstPart) + sizeof(serverIp) + sizeof(secondPartImage) + 1);

    serverCloseLink = malloc(sizeof(firstPart) + sizeof(serverIp) + sizeof(secondPartStop) + 1);

    strcpy(serverImageLink, firstPart);
    strcat(serverImageLink, serverIp);
    strcat(serverImageLink, secondPartImage);

    strcpy(serverCloseLink, firstPart);
    strcat(serverCloseLink, serverIp);
    strcat(serverCloseLink, secondPartStop);

    free(serverIp);

}

int stopServer() {

    int res;

    ulfius_init_request(&requestConnection);
        
    ulfius_set_request_properties(&requestConnection,
                                    U_OPT_HTTP_VERB, "GET",
                                    U_OPT_HTTP_URL, serverCloseLink,
                                    U_OPT_NONE);

    ulfius_init_response(&responseConnection);

    res = ulfius_send_http_request(&requestConnection, &responseConnection);
    if (res == U_OK) {
        printf("Successfully stopped the server and nodes\n");
    }
    else {
        printf("Failed to stop the server and nodes\n");
    }

    ulfius_clean_response(&responseConnection);
    ulfius_clean_request(&requestConnection);

    return 0;
}

int sendImageToServer() {

    int res;

    ulfius_init_request(&requestConnection);
        
    ulfius_set_request_properties(&requestConnection,
                                    U_OPT_HTTP_VERB, "POST",
                                    U_OPT_HTTP_URL, serverImageLink,
                                    U_OPT_JSON_BODY, jsonImage,
                                    U_OPT_NONE);

    ulfius_init_response(&responseConnection);

    res = ulfius_send_http_request(&requestConnection, &responseConnection);
    if (res == U_OK) {
        printf("Image sent server\n");
    }
    else {
        printf("Failed to send image to server\n");
        exit(EXIT_FAILURE);
    }

    ulfius_clean_response(&responseConnection);
    ulfius_clean_request(&requestConnection);

    return 0;
}