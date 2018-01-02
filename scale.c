#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel.
 *
 * Note that this function should allocate space only for a single Pixel;
 * do *not* store more than one Pixel at a time, it isn't necessary here!
 */
void scale_filter(Bitmap *bmp) {
    Pixel pixel_row[bmp->width * bmp->scale_factor];
    int counter;
    int error;
    for (int row = 0; row < bmp->height; row++){
        counter = 0;
        for (int column = 0; column < bmp->width; column++){
            Pixel pixel;
            error = 0;
            error += fread(&pixel.blue, sizeof(char), 1,stdin);
            error += fread(&pixel.green, sizeof(char), 1, stdin);
            error += fread(&pixel.red, sizeof(char), 1, stdin);
            if (error!=3){
                fprintf(stderr, "Could not read pixels\n");
                exit(1);
            }
            for (int repeat = 0; repeat <bmp->scale_factor;repeat++){
                pixel_row[counter] = pixel;
                counter++;
                error = fwrite(&pixel, sizeof(Pixel), 1, stdout);
                if(error!=1){
                    fprintf(stderr, "Could not read pixels\n");
                    exit(1);
                }
            }

        }
        for (int row_repeat = 1; row_repeat<bmp->scale_factor; row_repeat++){
            error = fwrite(pixel_row, sizeof(Pixel)*bmp->width * bmp->scale_factor, 1, stdout);
            if(error!=1){
                fprintf(stderr, "Could not read pixels\n");
                exit(1);
            }
        }
    }

    bmp->height = bmp->height * bmp->scale_factor;
    bmp->width = bmp->width * bmp->scale_factor;
}

int main(int argc, char** argv) {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    int scale = strtol(argv[1], NULL, 10);
    run_filter(scale_filter, scale);
    return 0;
}
