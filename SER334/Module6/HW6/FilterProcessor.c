/**
 * Filters Implementation
 *
 * Completion time: 12h
 *
 * @author Claudio Rodriguez Rodriguez
 * @version 02/08/2022
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "FilterProcessor.h"

#define DEBUG 0

struct Pixel_Slice {
    struct Pixel ** slice;
    int ** circles;
    int start;
    int end;
    int width;
    int height;
    int offset;
};

struct Pixel_Slice ** make_slices(struct Pixel ** pArr, int threads, int with_padding, int width, int height);
void * _box_blur_filter(void * arg);
void * _cheese_filter(void * arg);
void _copy_slices_into_image(struct Pixel ** pArr, struct Pixel_Slice ** slices, int threads, int width, int height);
void apply_threading(struct Pixel ** pArr, struct Pixel_Slice ** slices, int threads, int width, int height, void * ( * filter_func)(void * ));
int ** compute_holes(struct Pixel ** pArr, int width, int height);

void blur_filter(struct Pixel ** pArr, int max_threads, int width, int height) {
    printf("Applying Blur Box filter...\n");
    int thread_cap = max_threads < width ? max_threads : width;
    struct Pixel_Slice ** slices = make_slices(pArr, thread_cap, 1, width, height);
    apply_threading(pArr, slices, thread_cap, width, height, _box_blur_filter);

    if (DEBUG)
        printf("Deleting slices\n");

    int i = 0;
    int j = 0;
    for (i = 0; i < thread_cap; i++) {
        for (j = 0; j != height; j++) {
            free(slices[i]->slice[j]);
        }
        free(slices[i]->slice);
        free(slices[i]);
        slices[i] = NULL;
    }
    free(slices);
    slices = NULL;
}

void cheese_filter(struct Pixel ** pArr, int max_threads, int width, int height) {
    printf("Applying Cheese filter...\n");
    int thread_cap = max_threads < width ? max_threads : width;
    struct Pixel_Slice ** slices = make_slices(pArr, thread_cap, 0, width, height);
    int ** circles = compute_holes(pArr, width, height);
    if (circles == NULL) {
        return;
    }

    
    int i = 0;
    int j = 0;
    int k = 0;
    int z = 0;
    for (i = 0; i < max_threads; i++) {
        int ** slice_circles = (int ** ) malloc(sizeof(int*) * height);
        for (j = 0; j != height; j++) {
            slice_circles[j] = (int * ) malloc(sizeof(int) * slices[i]->width);
            for (k = 0; k < slices[i]->width; k++) {
                slices[i]->circles = slice_circles;
            }
        }
    }
    for (i = 0; i < max_threads; i++) {
        for (j = 0; j != height; j++) {
            z = 0;
            int start = slices[i]->offset;
            int end = slices[i]->offset + slices[i]->width;
            for (k = start; k != end; k++) {
                slices[i]->circles[j][z] = circles[j][k];
                z++;
            }
        }
    }

    apply_threading(pArr, slices, thread_cap, width, height, _cheese_filter);

    if (DEBUG) 
        printf("Deleting slices\n");

    for (i = 0; i < height; i++) {
        free(circles[i]);
    }
    free(circles);
    circles = NULL;

    for (i = 0; i < max_threads; i++) {
        for (j = 0; j != height; j++) {
            free(slices[i]->circles[j]);
            free(slices[i]->slice[j]);
        }
        free(slices[i]->slice);
        free(slices[i]->circles);
        free(slices[i]);
        slices[i] = NULL;
    }
    free(slices);
    slices = NULL;
}

int ** compute_holes(struct Pixel ** pArr, int width, int height) {
    int radius = 0;
    int all_holes = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    srand(time(NULL));
    if (width < height) {
        radius = (int)(width * 0.08);
        all_holes = (int)(width * 0.08);
    } else {
        radius = (int)(height * 0.08);
        all_holes = (int)(height * 0.08);
    }
    if (all_holes == 0) {
        return NULL;
    }
    int normal_holes = all_holes / 2;
    int small_holes = (int)(all_holes - normal_holes) * 0.7;
    int large_holes = (all_holes - normal_holes - small_holes);
    all_holes = normal_holes + small_holes + large_holes;

    // is not an exact science, but it works
    int s_radius = (int)radius * 0.7;
    int l_radius = (int)radius * 1.7;
    int m_radius = radius;
    
    if (DEBUG) {
        printf("normal_holes: %d\n", normal_holes);
        printf("small_holes: %d\n", small_holes);
        printf("large_holes: %d\n", large_holes);
        printf("small radius: %d\n", s_radius);
        printf("medium radius: %d\n", m_radius);
        printf("large radius: %d\n", l_radius);
    }

    int ** center = (int ** ) malloc(sizeof(int * ) * all_holes);
    for (i = 0; i != all_holes; ++i) {
        center[i] = (int * ) malloc(sizeof(int) * 2);
    }

    for (i = 0; i != all_holes; ++i) {
        int chunk_width;
        int chunk_height;
        int wMax = 0;
        int hMax = 0;
        int wMin = 0;
        int hMin = 0;
        char type[2] = {'\0', '\0'};
        if (i < normal_holes) {
            if (!normal_holes)
                return NULL;
            chunk_width = width / normal_holes;
            chunk_height = height / normal_holes;
            wMax = i * chunk_width;
            hMax = i * chunk_height;
            wMin = (i * chunk_width) + m_radius;
            hMin = (i * chunk_height) + m_radius;
            if (DEBUG)
                type[0] = 'm';
        } else if (i < normal_holes + small_holes) {
            if (!small_holes)
                break;
            chunk_width = width / small_holes;
            chunk_height = height / small_holes;
            wMax = i * chunk_width;
            hMax = i * chunk_height;
            wMin = (i * chunk_width) + s_radius;
            hMin = (i * chunk_height) + s_radius;
            if (DEBUG)
                type[0] = 's';
        } else {
            if (!large_holes)
                break;
            chunk_width = width / large_holes;
            chunk_height = height / large_holes;
            wMin = (i * chunk_width) + l_radius;
            hMin = (i * chunk_height) + l_radius;
            if (DEBUG)
                type[0] = 'l';
        }
        int nRandonNumberWidth = rand();
        int nRandonNumberHeight = rand();
        if (rand() % 1) {
            nRandonNumberWidth = rand()%((wMax+1)-wMin) + wMin;
        } else {
            nRandonNumberHeight = rand()%((hMax+1)-hMin) + hMin;
        }
        center[i][0] = nRandonNumberWidth % width;
        center[i][1] = nRandonNumberHeight % height;
        if (DEBUG) {
            printf("%s %d - %d %d\n", type, i , center[i][0], center[i][1]);
        }
    }


    int ** circles = (int ** ) malloc(sizeof(int * ) * height);
    for (i = 0; i != height; i++) {
        circles[i] = (int * ) malloc(sizeof(int) * width);
        for (int j = 0; j < width; j++) {
            circles[i][j] = 0;
        }
    }

    for (k = 0; k != all_holes; k++) {
        for (i = 0; i != height; ++i) {
            for (j = 0; j != width; ++j) {
                int x1 = center[k][0];
                int x2 = j;
                int y1 = center[k][1];
                int y2 = i;
                int dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
                if (k < normal_holes) {
                    if (dist < m_radius) {
                        circles[i][j] = 1;
                    }
                } else if (k < normal_holes + small_holes) {
                    if (dist < s_radius) {
                        circles[i][j] = 1;
                    }
                } else {
                    if (dist < l_radius) {
                        circles[i][j] = 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < radius; i++) {
        free(center[i]);
    }
    free(center);
    
    return circles;
}

void apply_threading(struct Pixel ** pArr, struct Pixel_Slice ** slices, int max_threads, int width, int height, void * ( * filter_func)(void * )) {
    pthread_t threads[max_threads];
    int threads_result[max_threads];
    int i = 0;

    for (i = 0; i != max_threads; i++) {
        threads_result[i] = pthread_create( & threads[i], NULL, filter_func, slices[i]);
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

    _copy_slices_into_image(pArr, slices, max_threads, width, height);
}

struct Pixel_Slice ** make_slices(struct Pixel ** pArr, int threads, int with_padding, int width, int height) {
    struct Pixel_Slice ** slices = malloc(sizeof(struct Pixel_Slice) * threads);
    int i = 0;
    int slice_width = (int) width / threads;
    int slice_residue = width % threads;
    int row = 0;
    int col = 0;
    // Build slices
    for (i = 0; i < threads; i++) {
        struct Pixel ** pixels = (struct Pixel ** ) malloc(sizeof(struct Pixel * ) * height);
        for (row = 0; row != height; row++) {
            if (i == threads - 1) {
                int padding = with_padding ? 1 : 0;
                pixels[row] = (struct Pixel * ) malloc(sizeof(struct Pixel) * (slice_width + slice_residue + padding));
            } else if (i == 0) {
                int padding = with_padding ? 1 : 0;
                pixels[row] = (struct Pixel * ) malloc(sizeof(struct Pixel) * (slice_width + padding));
            } else {
                int padding = with_padding ? 2 : 0;
                pixels[row] = (struct Pixel * ) malloc(sizeof(struct Pixel) * (slice_width + padding));
            }
        }
        slices[i] = malloc(sizeof(struct Pixel_Slice));
        slices[i]->slice = pixels;
        slices[i]->height = height;
    }
    // Fill and configure slices
    for (int i = 0; i < threads; i++) {
        // first slice
        if (i == 0) {
            int padding = with_padding ? 1 : 0;
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < slice_width + padding; k++) {
                    if (DEBUG && j == 0) {
                        printf("%d ", k);
                    }
                    slices[i]->slice[j][k].red = pArr[j][k].red;
                    slices[i]->slice[j][k].green = pArr[j][k].green;
                    slices[i]->slice[j][k].blue = pArr[j][k].blue;
                }
                if (DEBUG && j == 0) {
                    printf("\n");
                }
            }
            slices[i]->offset = 0;
            slices[i]->start = 0;
            slices[i]->end = slice_width + padding;
            slices[i]->width = slice_width + padding;
        }
        // last slice
        else if (i == threads - 1) {
            int padding = with_padding ? 1 : 0;
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < slice_width + padding + slice_residue; k++) {
                    if (DEBUG && j == 0) {
                        printf("%d ", k + i * slice_width - 1);
                    }
                    slices[i]->slice[j][k].red = pArr[j][k + i * slice_width - padding].red;
                    slices[i]->slice[j][k].green = pArr[j][k + i * slice_width - padding].green;
                    slices[i]->slice[j][k].blue = pArr[j][k + i * slice_width - padding].blue;
                }
                if (DEBUG && j == 0) {
                    printf("\n");
                }
            }
            slices[i]->offset = i * slice_width - padding;
            slices[i]->start = padding;
            slices[i]->end = slice_width + slice_residue + padding;
            slices[i]->width = slice_width + slice_residue + padding;
        }
        // mid slice
        else {
            int padding = with_padding ? 2 : 0;
            int start_padding = with_padding ? 1 : 0;
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < slice_width + padding; k++) {
                    if (DEBUG && j == 0) {
                        printf("%d ", k + i * slice_width - 1);
                    }
                    slices[i]->slice[j][k].red = pArr[j][k + i * slice_width - start_padding].red;
                    slices[i]->slice[j][k].green = pArr[j][k + i * slice_width - start_padding].green;
                    slices[i]->slice[j][k].blue = pArr[j][k + i * slice_width - start_padding].blue;
                }
                if (DEBUG && j == 0) {
                    printf("\n");
                }
            }
            slices[i]->offset = i * slice_width - start_padding;
            slices[i]->start = start_padding;
            slices[i]->end = slice_width + padding;
            slices[i]->width = slice_width + padding;
        }
    }

    if (DEBUG) {
        for (i = 0; i < threads; i++) {
            printf("%d: offset: %d, start: %d, end: %d, width: %d\n", i, slices[i]->offset, slices[i]->start, slices[i]->end, slices[i]->width);
        }
    }

    return slices;
}

void _copy_slices_into_image(struct Pixel ** pArr, struct Pixel_Slice ** slices, int threads, int width, int height) {
    int slice_width = (int) width / threads;
    int slice_residue = width % threads;

    for (int i = 0; i < threads; i++) {
        for (int j = 0; j != slices[i]->height; j++) {
            int row_width = i < threads - 1 ? slice_width : slice_width + slice_residue;
            row_width += slices[i]->start;
            if (DEBUG && j == 0) {
                printf("row_width: %d\n", row_width);
            }
            int offset = slices[i]->offset;
            for (int k = slices[i]->start; k != row_width; k++) {
                if (DEBUG && j == 0) {
                    if (k == slices[i]->start) {
                        printf("0 - ");
                    }
                    printf("%d ", offset + k);
                }
                if (DEBUG && j == 1) {
                    if (k == slices[i]->start) {
                        printf("1 - ");
                    }
                    printf("%d ", k);
                }
                pArr[j][offset + k].red = slices[i]->slice[j][k].red;
                pArr[j][offset + k].green = slices[i]->slice[j][k].green;
                pArr[j][offset + k].blue = slices[i]->slice[j][k].blue;
            }
            if (DEBUG && (j == 0 || j == 1)) {
                printf("\n");
            }

        }
    }
}

void * _box_blur_filter(void * arg) {
    struct Pixel_Slice * slice = (struct Pixel_Slice * ) arg;
    if (DEBUG) {
        printf("start %d, end %d, width %d, height %d\n", slice->start, slice->end, slice->width, slice->height);
    }
    int i = 0;
    int j = 0;
    for (i = 0; i != slice->height; ++i) {
        for (j = slice->start; j != slice->end; ++j) {
            struct Pixel p1, p2, p3;
            struct Pixel p4, p5, p6;
            struct Pixel p7, p8, p9;
            int count = 0;
            p1.red = 0;
            p2.red = 0;
            p3.red = 0;
            p1.green = 0;
            p2.green = 0;
            p3.green = 0;
            p1.blue = 0;
            p2.blue = 0;
            p3.blue = 0;

            p4.red = 0;
            p5.red = 0;
            p6.red = 0;
            p4.green = 0;
            p5.green = 0;
            p6.green = 0;
            p4.blue = 0;
            p5.blue = 0;
            p6.blue = 0;

            p7.red = 0;
            p8.red = 0;
            p9.red = 0;
            p7.green = 0;
            p8.green = 0;
            p9.green = 0;
            p7.blue = 0;
            p8.blue = 0;
            p9.blue = 0;

            p5.red = slice->slice[i][j].red;
            p5.green = slice->slice[i][j].green;
            p6.blue = slice->slice[i][j].blue;
            count++;

            // top left
            if (i - 1 >= 0 && j - 1 >= 0) {
                p1.red = slice->slice[i - 1][j - 1].red;
                p1.green = slice->slice[i - 1][j - 1].green;
                p1.blue = slice->slice[i - 1][j - 1].blue;
                count++;
            }
            // top
            if (i - 1 >= 0) {
                p2.red = slice->slice[i - 1][j].red;
                p2.green = slice->slice[i - 1][j].green;
                p2.blue = slice->slice[i - 1][j].blue;
                count++;
            }
            // top right
            if (i - 1 >= 0 && j + 1 < slice->width) {
                p3.red = slice->slice[i - 1][j + 1].red;
                p3.green = slice->slice[i - 1][j + 1].green;
                p3.blue = slice->slice[i - 1][j + 1].blue;
                count++;
            }
            // left
            if (j - 1 >= 0) {
                p4.red = slice->slice[i][j - 1].red;
                p4.green = slice->slice[i][j - 1].green;
                p4.blue = slice->slice[i][j - 1].blue;
                count++;
            }
            // right
            if (j + 1 < slice->width) {
                p6.red = slice->slice[i][j + 1].red;
                p6.green = slice->slice[i][j + 1].green;
                p6.blue = slice->slice[i][j + 1].blue;
                count++;
            }
            // bottom left
            if (i + 1 < slice->height && j - 1 >= 0) {
                p7.red = slice->slice[i + 1][j - 1].red;
                p7.green = slice->slice[i + 1][j - 1].green;
                p7.blue = slice->slice[i + 1][j - 1].blue;
                count++;
            }
            // bottom
            if (i + 1 < slice->height) {
                p8.red = slice->slice[i + 1][j].red;
                p8.green = slice->slice[i + 1][j].green;
                p8.blue = slice->slice[i + 1][j].blue;
                count++;
            }
            // bottom right
            if (i + 1 < slice->height && j + 1 < slice->width) {
                p9.red = slice->slice[i + 1][j + 1].red;
                p9.green = slice->slice[i + 1][j + 1].green;
                p9.blue = slice->slice[i + 1][j + 1].blue;
                count++;
            }

            slice->slice[i][j].red = (p1.red + p2.red + p3.red + p4.red + p5.red + p6.red + p7.red + p8.red + p9.red) / count;
            slice->slice[i][j].green = (p1.green + p2.green + p3.green + p4.green + p5.green + p6.green + p7.green + p8.green + p9.green) / count;
            slice->slice[i][j].blue = (p1.blue + p2.blue + p3.blue + p4.blue + p5.blue + p6.blue + p7.blue + p8.blue + p9.blue) / count;
        }
    }
    return NULL;
}

void * _cheese_filter(void * arg) {
    struct Pixel_Slice * slice = (struct Pixel_Slice * ) arg;
    int i = 0;
    int j = 0;
    
    for (i = 0; i != slice->height; ++i) {
        for (j = slice->start; j != slice->end; ++j) {
            if (slice->circles[i][j] == 1) {
                slice->slice[i][j].red = 0;
                slice->slice[i][j].green = 0;
                slice->slice[i][j].blue = 0;
            }
        }
    }

    colorShiftPixels(slice->slice, slice->width, slice->height, 20, 20, -20, 1);
}