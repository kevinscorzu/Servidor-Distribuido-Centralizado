#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

    int imagesToSend = 0;
    char xorKey[5];
    char imageName[50];

    strcpy(xorKey, "");
    strcpy(imageName, "");

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            imagesToSend = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-k") == 0) {
            strcpy(xorKey, argv[i + 1]);
        }
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(imageName, argv[i + 1]);
        }
    }

    if (imagesToSend == 0 || strcmp(xorKey, "") == 0 || strcmp(imageName, "") == 0) {
        printf("Error, no se puedo determinar la cantidad de imagenes a enviar, la llave de la imagen o el nombre de la imagen\n");
        return 1;
    }

    

    return 0;
}