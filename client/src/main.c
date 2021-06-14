#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/config.h"
#include "client/client.h"

int main(int argc, char* argv[]) {

    int imagesToSend = 0;
    int xorKey = 0;
    char imageName[50];
    int closeServer = 0;

    strcpy(imageName, "");

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            imagesToSend = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-k") == 0) {
            xorKey = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(imageName, argv[i + 1]);
        }
        if (strcmp(argv[i], "-e") == 0) {
            closeServer = 1;
        }
    }

    readConfigFile();
    makeIp();

    if (closeServer == 1) {
        stopServer();

        free(serverCloseLink);
        free(serverImageLink);
        return 0;
    }

    if (imagesToSend == 0 || xorKey == 0 || strcmp(imageName, "") == 0) {
        printf("Error, couldn't determine the amount of images to send, the image decryption key or the image name\n");
        return 1;
    }

    // Aqui vendria lectura de imagen a base64

    jsonImage = json_object();
    json_object_set_new(jsonImage, "image", json_string("abcdefghijklmnopqrstuvxyz"));
    json_object_set_new(jsonImage, "key", json_integer(xorKey));

    int i = 0;
    int userInput;
    while (1) {
        if (i == imagesToSend) {
            printf("All the images where sent\n");
            printf("If you want to finish, write 0, if you want to send more images, write the amount of images to send\n");
            scanf("%d", &userInput);
            if (userInput == 0) {
                break;
            }
            else {
                i = 0;
                imagesToSend = userInput;        
            }
        }

        i++;
        printf("Sending image number %d\n", i);
        sendImageToServer();
    }

    json_decref(jsonImage);
    free(serverCloseLink);
    free(serverImageLink);

    return 0;
}