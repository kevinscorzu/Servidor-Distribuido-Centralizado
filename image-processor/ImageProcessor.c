#include <stdio.h>
#include <stdlib.h>
//Comando para ejecutar: gcc -std=c17 -Wall -pedantic ImageProcessor.c -o ImPro -lm
 
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "../stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

int main(void) {
    int width, height, channels;
    unsigned char *img = stbi_load("10-10.jpg", &width, &height, &channels, 0);

    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    int img_size = width * height * channels;
    
    int key = 10;

    unsigned char imgResult[img_size];

    unsigned char* begin = img;
    unsigned char* end = imgResult;

    for (int i = 0; i <= img_size; i++) {
        *end = *begin ^ key;
        end++;
        begin++;
    }

    stbi_write_jpg("10-10-10.jpg", width, height, channels, imgResult, 100);

    return 0;

    // for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += channels) {
    //     *pg       = (uint8_t)(*p ^ 54);         // red
    //     *(pg + 1) = (uint8_t)(*(p + 1) ^ 54);         // green
    //     *(pg + 2) = (uint8_t)(*(p + 2) ^ 54);         //blue
    //     if(channels == 4) {
    //         *(pg + 3) = (uint8_t)(*(p + 3) ^ 54);
    //     }
    // }
    //stbi_write_jpg("combination2.jpg", width, height, channels, gray_img, 100);
}