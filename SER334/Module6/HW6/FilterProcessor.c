#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "FilterProcessor.h"

#define DEBUG 1
#define FILTER_WIDTH 3
#define FILTER_HEIGHT 3

struct Pixel_Slice {
    struct Pixel **slice;
    int start;
    int end;
    int width;
    int height;
};

struct Pixel_Slice** make_slices(struct Pixel** pArr, int threads, int with_padding, int width, int height);
void _box_blur_filter(struct Pixel_Slice* slice, int width, int height);
void _cheese_filter(struct Pixel_Slice* slice, int width, int height);

void blur_filter(struct Pixel** pArr, int threads, int width, int height) {
    printf("Applying Blur Box filter...\n");
    int thread_cap = threads < width ? threads : width;
    struct Pixel_Slice **slices = make_slices(pArr, thread_cap, 1, width, height);
    if (DEBUG) {
        int i = 0;
        for (i = 0; i != thread_cap; i++) {
            printf("start %d, end %d, width %d, height %d\n", slices[i]->start, slices[i]->end, slices[i]->width, slices[i]->height);
        }
    }
    
}

void cheese_filter(struct Pixel** pArr, int threads, int width, int height) {
    printf("Applying Cheese filter...\n");
    int thread_cap = threads < width ? threads : width;
    struct Pixel_Slice **slices = make_slices(pArr, thread_cap, 0, width, height);
    if (DEBUG) {
        int i = 0;
        for (i = 0; i != thread_cap; i++) {
            printf("start %d, end %d, width %d, height %d\n", slices[i]->start, slices[i]->end, slices[i]->width, slices[i]->height);
        }
    }

}



struct Pixel_Slice** make_slices(struct Pixel** pArr, int threads, int with_padding, int width, int height) {
    struct Pixel_Slice **slices = malloc(sizeof(struct Pixel_Slice) * threads);
    int i = 0;

    for (i = 0; i < threads; i++) {
        int row = 0;
        int col = 0;
        int row_width = i < threads - 1 ? (int)floor(width / (double)threads) : (int)ceil(width / (double)threads);
        int end = row_width;
        int offset = i * row_width;
        
        int padding_x_left = with_padding && i != 0 ? 1 : 0;
        int padding_x_right = with_padding && i != threads - 1 ? 1 : 0;
        row_width = row_width + padding_x_left + padding_x_right;
        
        struct Pixel** pixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * height);
        
        for (int row = 0; row != height; row++) {
            pixels[row] = (struct Pixel*)malloc(sizeof(struct Pixel) * row_width);
            for (int col = 0; col != row_width; col++) {
                pixels[row][col].red = pArr[row][offset+col].red;
                pixels[row][col].green = pArr[row][offset+col].green;
                pixels[row][col].blue = pArr[row][offset+col].blue;
            }
        }
        slices[i] = malloc(sizeof(struct Pixel_Slice));
        slices[i]->slice = pixels;
        slices[i]->start = padding_x_left;
        slices[i]->end = end;
        slices[i]->width = row_width;
        slices[i]->height = height;
    }

    return slices;
}

void _box_blur_filter(struct Pixel_Slice* slice, int width, int height) {

}
void _cheese_filter(struct Pixel_Slice* slice, int width, int height) {

}