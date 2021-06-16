#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "../../../stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../stb/stb_image_write.h"

void makeIp();
void *toAnalize(void *arg);
int decryptImage(int key, char* imageName);
void confirmImageDone(int nodeId, int threadId);

/**
 * Function that initializes the threads that will handle the images
 */
void initializeImageHandler() {

    makeIp();

    pthread_create(&t0, NULL, toAnalize, (void*) (__intptr_t) 1);
    pthread_create(&t1, NULL, toAnalize, (void*) (__intptr_t) 2);
    pthread_create(&t2, NULL, toAnalize, (void*) (__intptr_t) 3);

}

/**
 * Function that make the ip to connect to the server
 */
void makeIp() {

    char *firstPart = "http://";
    char *secondPart = "/Api/Confirm";

    serverLink = malloc(strlen(firstPart) + strlen(serverIp) + strlen(secondPart) + 1);

    strcpy(serverLink, firstPart);
    strcat(serverLink, serverIp);
    strcat(serverLink, secondPart);

    free(serverIp);

}

/**
 * Function that handles an image
 */
void *toAnalize(void *arg) {
    int tid = (int) (__intptr_t) arg;

    int keyLocal = 0;
    char* imageLocal;
    char* imageNameLocalTXT;
    char* imageNameLocalJPG;
    char* decodeCommand;

    while(1){
        waitSemaphore(tid);
        if (closeNode == 1) {
            break;
        }

        switch (tid) {
        case 1:
            imageNameLocalTXT = malloc(strlen(imageName0) + strlen(".txt") + 1);
            strcpy(imageNameLocalTXT, imageName0);
            strcat(imageNameLocalTXT, ".txt");

            imageNameLocalJPG = malloc(strlen(imageName0) + 1);
            strcpy(imageNameLocalJPG, imageName0);

            imageLocal = image0;
            keyLocal = key0;
            break;
        case 2:
            imageNameLocalTXT = malloc(strlen(imageName0) + strlen(".txt") + 1);
            strcpy(imageNameLocalTXT, imageName1);
            strcat(imageNameLocalTXT, ".txt");

            imageNameLocalJPG = malloc(strlen(imageName1) + 1);
            strcpy(imageNameLocalJPG, imageName1);

            imageLocal = image1;
            keyLocal = key1;
            break;
        case 3:
            imageNameLocalTXT = malloc(strlen(imageName0) + strlen(".txt") + 1);
            strcpy(imageNameLocalTXT, imageName2);
            strcat(imageNameLocalTXT, ".txt");

            imageNameLocalJPG = malloc(strlen(imageName2) + 1);
            strcpy(imageNameLocalJPG, imageName2);

            imageLocal = image2;
            keyLocal = key2;
            break;
        }

        FILE *file = fopen(imageNameLocalTXT, "w");
        if (!file) {
            printf("Unable to create file %s", imageNameLocalTXT);
            exit(EXIT_FAILURE);
        }

        fprintf(file, "%s", imageLocal);
        fclose(file);

        decodeCommand = malloc(strlen(imageNameLocalTXT) + strlen(imageNameLocalJPG) + strlen("base64 --decode  > ") + 1);
        strcpy(decodeCommand, "base64 --decode ");
        strcat(decodeCommand, imageNameLocalTXT);
        strcat(decodeCommand, " > ");
        strcat(decodeCommand, imageNameLocalJPG);

        system(decodeCommand);
        remove(imageNameLocalTXT);

        decryptImage(keyLocal, imageNameLocalJPG);

        free(imageNameLocalTXT);
        free(imageNameLocalJPG);
        free(imageLocal);
        confirmImageDone(id, tid);
    }

    pthread_exit(NULL);
}

/**
 * Function that decrypts an image
 */
int decryptImage(int key, char* imageName) {
    int width, height, channels;
    unsigned char *oldImage = stbi_load(imageName, &width, &height, &channels, 0);

    if(oldImage == NULL) {
        printf("Error loading the image\n");
        exit(EXIT_FAILURE);
    }
    int img_size = width * height * channels;

    unsigned char newImage[img_size];
    unsigned char* p = newImage;

    for (int i = 0; i < img_size; i++) {
        *p = *oldImage ^ key;
        p += 1;
        oldImage += 1;
    }

    remove(imageName);

    stbi_write_jpg(imageName, width, height, channels, newImage, 100);
    
    return 0;
}

/**
 * Function alerts the server that it finished an image
 */
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