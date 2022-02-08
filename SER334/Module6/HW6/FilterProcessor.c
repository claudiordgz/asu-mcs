#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
    int offset;
    int slice_width;
    int slice_height;
};

struct Pixel_Slice** make_slices(struct Pixel** pArr, int threads, int with_padding, int width, int height);
void* _box_blur_filter(void *arg);
void* _cheese_filter(void *arg);
void _copy_slices_into_image(struct Pixel** pArr, struct Pixel_Slice** slices, int threads);
void apply_threading(struct Pixel** pArr, struct Pixel_Slice** slices, int threads, void* (*filter_func)(void *));

void blur_filter(struct Pixel** pArr, int max_threads, int width, int height) {
    printf("Applying Blur Box filter...\n");
    int thread_cap = max_threads < width ? max_threads : width;
    struct Pixel_Slice **slices = make_slices(pArr, thread_cap, 1, width, height);
    apply_threading(pArr, slices, thread_cap, _box_blur_filter);
    
    if (DEBUG) 
        printf("Deleting slices\n");
    
    for (int i = 0; i < max_threads; i++) {
        free(slices[i]);
        slices[i] = NULL;
    }
    free(slices);
    slices = NULL;
}

void cheese_filter(struct Pixel** pArr, int max_threads, int width, int height) {
    printf("Applying Cheese filter...\n");
    int thread_cap = max_threads < width ? max_threads : width;
    struct Pixel_Slice **slices = make_slices(pArr, thread_cap, 0, width, height);
    apply_threading(pArr, slices, thread_cap, _cheese_filter);

    if (DEBUG) 
        printf("Deleting slices\n");
    
    for (int i = 0; i < max_threads; i++) {
        free(slices[i]);
        slices[i] = NULL;
    }
    free(slices);
    slices = NULL;
}

void apply_threading(struct Pixel** pArr, struct Pixel_Slice** slices, int max_threads, void* (*filter_func)(void *)) {
    pthread_t threads[max_threads];
    int threads_result[max_threads];
    int i = 0;
    for (i = 0; i != max_threads; i++) {
        threads_result[i] = pthread_create(&threads[i], NULL, filter_func, slices[i]);
        if (threads_result[i] != 0) {
            printf("ERORR = pthread_create() returned %d\n", threads_result[i]);
            exit(EXIT_FAILURE);
        }
    }
    void* retval;
    for (int i = 0; i < max_threads; i++) {
        pthread_join(threads[i], &retval);
    }
    if (DEBUG) 
        printf("Slices to image\n");
    
    _copy_slices_into_image(pArr, slices, max_threads);
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
        slices[i]->offset = offset;
        slices[i]->slice = pixels;
        slices[i]->start = padding_x_left;
        slices[i]->end = end;
        slices[i]->width = row_width;
        slices[i]->height = height;
    }
    if (DEBUG){
        for (i = 0; i < threads; i++) {
            printf("%d: offset: %d, start: %d, end: %d, width: %d\n", i, slices[i]->offset, slices[i]->start, slices[i]->end, slices[i]->width);
        }
    }

    return slices;
}

void _copy_slices_into_image(struct Pixel** pArr, struct Pixel_Slice** slices, int threads) {
    printf("copying\n");
    for (int i = 0; i < threads; i++) {
        for (int row = 0; row != slices[i]->height; row++) {
            for (int col = slices[i]->start; col != slices[i]->end; col++) {
                pArr[row][slices[i]->offset + col - slices[i]->start].red = slices[i]->slice[row][col].red;
                pArr[row][slices[i]->offset + col - slices[i]->start].green = slices[i]->slice[row][col].green;
                pArr[row][slices[i]->offset + col - slices[i]->start].blue = slices[i]->slice[row][col].blue;
            }
        }
    }
}

void* _box_blur_filter(void *arg) {
    struct Pixel_Slice *slice = (struct Pixel_Slice *)arg;
    printf("start %d, end %d, width %d, height %d\n", slice->start, slice->end, slice->width, slice->height);
    int row = 0;
    int col = 0;
    for (row = 0; row != slice->height; ++row) {
        for (col = slice->start; col != slice->end; ++col) {
            struct Pixel p1, p2, p3;
            struct Pixel p4, p5, p6;
            struct Pixel p7, p8, p9;
            int count = 0;
            p1.red = 0; p2.red = 0; p3.red = 0;
            p1.green = 0; p2.green = 0; p3.green = 0;
            p1.blue = 0; p2.blue = 0; p3.blue = 0;
            
            p4.red = 0; p5.red = 0; p6.red = 0;
            p4.green = 0; p5.green = 0; p6.green = 0;
            p4.blue = 0; p5.blue = 0; p6.blue = 0;

            p7.red = 0; p8.red = 0; p9.red = 0;
            p7.green = 0; p8.green = 0; p9.green = 0;
            p7.blue = 0; p8.blue = 0; p9.blue = 0;
            
            p5.red = slice->slice[row][col].red;
            p5.green = slice->slice[row][col].green;
            p6.blue = slice->slice[row][col].blue;
            count++;

            // top left
            if (row - 1 >= 0 && col - 1 >= 0) {
                p1.red = slice->slice[row - 1][col - 1].red;
                p1.green = slice->slice[row - 1][col - 1].green;
                p1.blue = slice->slice[row - 1][col - 1].blue;
                count++;
            }
            // top
            if (row - 1 >= 0) {
                p2.red = slice->slice[row - 1][col].red;
                p2.green = slice->slice[row - 1][col].green;
                p2.blue = slice->slice[row - 1][col].blue;
                count++;
            }
            // top right
            if (row - 1 >= 0 && col + 1 < slice->width - 1) {
                p3.red = slice->slice[row - 1][col + 1].red;
                p3.green = slice->slice[row - 1][col + 1].green;
                p3.blue = slice->slice[row - 1][col + 1].blue;
                count++;
            }
            // left
            if (col - 1 >= 0) {
                p4.red = slice->slice[row][col - 1].red;
                p4.green = slice->slice[row][col - 1].green;
                p4.blue = slice->slice[row][col - 1].blue;
                count++;
            }
            // right
            if (col + 1 < slice->width - 1) {
                p6.red = slice->slice[row][col + 1].red;
                p6.green = slice->slice[row][col + 1].green;
                p6.blue = slice->slice[row][col + 1].blue;
                count++;
            }
            // bottom left
            if (row + 1 < slice->height && col - 1 >= 0) {
                p7.red = slice->slice[row + 1][col - 1].red;
                p7.green = slice->slice[row + 1][col - 1].green;
                p7.blue = slice->slice[row + 1][col - 1].blue;
                count++;
            }
            // bottom
            if (row + 1 < slice->height) {
                p8.red = slice->slice[row + 1][col].red;
                p8.green = slice->slice[row + 1][col].green;
                p8.blue = slice->slice[row + 1][col].blue;
                count++;
            }
            // bottom right
            if (row + 1 < slice->height && col + 1 < slice->width - 1) {
                p9.red = slice->slice[row + 1][col + 1].red;
                p9.green = slice->slice[row + 1][col + 1].green;
                p9.blue = slice->slice[row + 1][col + 1].blue;
                count++;
            }
            
            slice->slice[row][col].red = (p1.red + p2.red + p3.red + p4.red + p5.red + p6.red + p7.red + p8.red + p9.red) / count;
            slice->slice[row][col].green = (p1.green + p2.green + p3.green + p4.green + p5.green + p6.green + p7.green + p8.green + p9.green) / count;
            slice->slice[row][col].blue = (p1.blue + p2.blue + p3.blue + p4.blue + p5.blue + p6.blue + p7.blue + p8.blue + p9.blue) / count;  
        }
    }
    return NULL;
}

void* _cheese_filter(void *arg) {
    struct Pixel_Slice *slice = (struct Pixel_Slice *)arg;
    printf("start %d, end %d, width %d, height %d\n", slice->start, slice->end, slice->width, slice->height);
}