#include <stdio.h>
#include <stdlib.h>
//Comando para ejecutar: gcc -std=c17 -Wall -pedantic ImageProcessor.c -o ImPro -lm
 
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main(void) {
    int width, height, channels,width_h,height_h,channels_h;
    unsigned char *img = stbi_load("manzana.jpg", &width, &height, &channels, 0);
    unsigned char *img_h = stbi_load("ciudad.jpg", &width_h, &height_h, &channels_h, 0);
    if((img == NULL) & (img_h==NULL)) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    size_t img_size = width * height * channels;
 
    unsigned char *gray_img = malloc(img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(unsigned char *p = img, *h =img_h,*pg = gray_img; p != img + img_size; p += channels,h += channels_h, pg += channels) {
        // *pg       = (uint8_t)fmin(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2), 255.0);         // red
        // *(pg + 1) = (uint8_t)fmin(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2), 255.0);         // green
        // *(pg + 2) = (uint8_t)fmin(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2), 255.0);         // blue
        *pg       = (uint8_t)(*p ^ *h);         // red
        *(pg + 1) = (uint8_t)(*(p + 1) ^ *(h + 1));         // green
        *(pg + 2) = (uint8_t)(*(p + 2) ^ *(h + 2));         //blue
        if(channels == 4) {
            *(pg + 3) = (uint8_t)(*(p + 3) ^ *(h + 3));
        }
    }
    stbi_write_jpg("combination.jpg", width, height, channels, gray_img, 100);
}