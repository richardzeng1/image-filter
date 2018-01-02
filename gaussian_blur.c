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
void gaussian_blur_filter(Bitmap *bmp) {
    int error;
    Pixel *row1= malloc(sizeof(Pixel)*bmp->width);
    Pixel *row2 = malloc(sizeof(Pixel)*bmp->width);
    Pixel *row3 = malloc(sizeof(Pixel)*bmp->width);
    Pixel new_pixel;
    Pixel result;

    Pixel *initial_rows[3] = {row1, row2, row3};
    // Read in initial rows
    for (int row = 0; row < 3; row++){
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
            initial_rows[row][column] = pixel;
        }
    }

    for (int row = 0; row<bmp->height; row++){
        for (int column = 0; column < bmp->width; column++){
            // column value is the pixel column we want to transform
            if (column ==0){
                result = apply_gaussian_kernel(row1, row2, row3);
            }else if (column == bmp->width-1){
                result = apply_gaussian_kernel(row1+((column-2)), row2+((column-2)), row3+((column-2)));
            }else{
                result = apply_gaussian_kernel(row1+((column-1)), row2+((column-1)), row3+((column-1)));
            }

            error = fwrite(&result, sizeof(Pixel), 1, stdout);
            if (error !=1){
                fprintf(stderr, "Pixel not written\n");
                exit(1);
            }

        }
        if ((row>=1) && (row < bmp->height-2)){
            // reading in the new row
            for (int column = 0; column < bmp->width; column++){
                new_pixel = row1[column];
                error = 0;
                error += fread(&new_pixel.blue, sizeof(char), 1,stdin);
                error += fread(&new_pixel.green, sizeof(char), 1, stdin);
                error += fread(&new_pixel.red, sizeof(char), 1, stdin);
                if (error!=3){
                    fprintf(stderr, "Could not read pixels\n");
                    exit(1);
                }
                row1[column] = new_pixel;
            }

            // Shifting 2 and 3 up by one and setting 1 as the third.
            Pixel * temp = row1;
            row1 = row2;
            row2 = row3;
            row3 = temp;
        }
    }
    free(row1);
    free(row2);
    free(row3);
}

int main() {
    // Run the filter program with copy_filter to process the pixels.
    // You shouldn't need to change this implementation.
    run_filter(gaussian_blur_filter, 1);
    return 0;
}
