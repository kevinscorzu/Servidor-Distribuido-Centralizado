#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/config.h"
#include "client/client.h"

char *base64_encode_string;
const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int encondeImage(char *imageName);
size_t b64_encoded_size(size_t inlen);
unsigned char *b64_encode(const unsigned char *in, size_t len);

int main(int argc, char* argv[]) {

    int imagesToSend = 0;
    int xorKey = 0;
    char imageName[50];
    char imagePath[100];
    char trueImagePath[100];
    int closeServer = 0;

    strcpy(imageName, "");
    strcpy(imagePath, "images/");
    strcpy(trueImagePath, "images/");

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

    strcat(trueImagePath, imageName);
    if (encondeImage(trueImagePath) == 1) {
        return 1;
    }

    jsonImage = json_object();
    json_object_set_new(jsonImage, "image", json_string(base64_encode_string));
    json_object_set_new(jsonImage, "key", json_integer(xorKey));
    json_object_set_new(jsonImage, "name", json_string(""));

    int i = 0;
    int userInput;
    char iChar[5];
    while (1) {
        if (i == imagesToSend) {
            printf("All the images where sent\n");
            printf("If you want to finish, write 0, if you want to send more images, write the amount of images to send\n");
            scanf("%d", &userInput);
            if (userInput == 0) {
                break;
            }
            else {
                imagesToSend += userInput;        
            }
        }

        i++;

        printf("Sending image number %d\n", i);
        sprintf(iChar, "%d", i);
        strcat(imagePath, iChar);
        strcat(imagePath, "-");
        strcat(imagePath, imageName);

        json_object_set(jsonImage, "name", json_string(imagePath));

        sendImageToServer();
    }

    json_decref(jsonImage);
    free(serverCloseLink);
    free(serverImageLink);

    return 0;
}

int encondeImage(char *imageName) {
    FILE *file;
	size_t b64_decode_len;
    size_t fileLen=0;
    unsigned char *buffer;

    file = fopen(imageName , "rb");

    if (!file) {
        printf("Unable to open file %s\n", imageName);
        return 1;
    }

    //Get file length                                                                                                                                                                                          
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);

    //Allocate memory                                                                                                                                                                                          
    buffer = (char*) malloc(fileLen + 1);
    if (!buffer) {
      printf("Memory error!\n");
      fclose(file);
      return 1;
    }

    //Read file contents into buffer  
    fread(buffer, fileLen, 1, file);
    base64_encode_string = b64_encode((const unsigned char *)buffer, fileLen);

    free(buffer);
    fclose(file);
    return 0;
}

size_t b64_encoded_size(size_t inlen) {
	size_t ret;

	ret = inlen;
	if (inlen % 3 != 0)
		ret += 3 - (inlen % 3);
	ret /= 3;
	ret *= 4;

	return ret;
}

unsigned char *b64_encode(const unsigned char *in, size_t len) {
  
	unsigned char *out;
	size_t  elen;
	size_t  i;
	size_t  j;
	size_t  v;

	if (b64chars == NULL || len == 0)
		return NULL;

	elen = b64_encoded_size(len);
	out  = malloc(elen+1);
	out[elen] = '\0';

	for (i=0, j=0; i<len; i+=3, j+=4) {
		v = in[i];
		v = i+1 < len ? v << 8 | in[i+1] : v << 8;
		v = i+2 < len ? v << 8 | in[i+2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j+1] = b64chars[(v >> 12) & 0x3F];
		if (i+1 < len) {
			out[j+2] = b64chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < len) {
			out[j+3] = b64chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return out;
}