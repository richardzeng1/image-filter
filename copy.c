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
void copy_filter(Bitmap *bmp) {
    int error;
    Pixel pixel;

    for (int row = 0; row < bmp->height; row++){
        for (int column = 0; column < bmp->width; column++){
            error = 0;
            error += fread(&pixel.blue, sizeof(char), 1,stdin);
            error += fread(&pixel.green, sizeof(char), 1, stdin);
            error += fread(&pixel.red, sizeof(char), 1, stdin);
            if (error!=3){
                fprintf(stderr, "Could not read pixels\n");
                exit(1);
            }
            error = fwrite(&pixel, sizeof(Pixel), 1, stdout);
            if (error!=1){
                fprintf(stderr, "Pixel not written\n");
                exit(1);
            }
        }
    }
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(copy_filter, 1);
    return 0;
}
