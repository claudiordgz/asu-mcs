#include "Image.h"

Image* image_create(struct Pixel** pArr, int width, int height) {
    Image *img = (Image *) malloc(sizeof (Image));
    if (img == NULL) {
        printf("Failed to create Image.\n");
        exit(1);
    }
    img->pArr = pArr;
    img->width = width;
    img->height = height;

    return img;
}


void image_destroy(Image** img) {
    free(*img);
    *img = NULL;
}

struct Pixel** image_get_pixels(Image* img) {
    return img->pArr;
}

int image_get_width(Image* img) {
    return img->width;
}

int image_get_height(Image* img) {
    return img->height;
}

void image_apply_bw(Image* img) {
    double grayscale = 0.0;
    for (int i = 0; i != img->height; i++) {
        for (int j = 0; j != img->width; j++) {
            grayscale = 0.299 * img->pArr[i][j].red + 0.587 * img->pArr[i][j].green + 0.114 * img->pArr[i][j].blue;
            if (grayscale > 255) {
                grayscale = 255;
            } else if (grayscale < 0) {
                grayscale = 0;
            }
            img->pArr[i][j].red = (unsigned char)grayscale;
            img->pArr[i][j].green = (unsigned char)grayscale;
            img->pArr[i][j].blue = (unsigned char)grayscale;
        }
    }
}

void image_apply_colorshift(Image* img, int rShift, int gShift, int bShift) {
    for (int i = 0; i != img->height; i++) {
        for (int j = 0; j != img->width; j++) {
            if (rShift != 0) {
                if (img->pArr[i][j].red + rShift > 255) {
                    img->pArr[i][j].red = 255;
                } else if (img->pArr[i][j].red + rShift < 0) {
                    img->pArr[i][j].red = 0;
                } else {
                    img->pArr[i][j].red += rShift;
                }
            }
            if (gShift != 0) {
                if (img->pArr[i][j].green + gShift > 255) {
                    img->pArr[i][j].green = 255;
                } else if (img->pArr[i][j].green + gShift < 0) {
                    img->pArr[i][j].red = 0;
                } else {
                    img->pArr[i][j].green += gShift;
                }
            }
            if (bShift != 0) {
                if (img->pArr[i][j].blue + bShift > 255) {
                    img->pArr[i][j].blue = 255;
                } else if (img->pArr[i][j].blue + bShift < 0) {
                    img->pArr[i][j].blue = 0;
                } else {
                    img->pArr[i][j].blue += bShift;
                }
            }
        }
    }
}

void image_apply_resize(Image* img, float factor) {
    double new_widthf = (double)img->width * factor;
    double new_heightf =(double)img->height * factor;
    int new_width = (int)new_widthf;
    int new_height = (int)new_heightf;
    double x_ratio = (double)img->width / new_widthf;
    double y_ratio = (double)img->height / new_heightf;

    Pixel** pixels = (Pixel**)malloc(sizeof(Pixel*) * new_height);
    for (int p = 0; p != new_height; p++) {
        pixels[p] = (Pixel*)malloc(sizeof(Pixel) * new_width);
    }

    int i = 0;
    int j = 0;
    double x,y = 0;
    for (i = 0; i != new_height; i++) {
        for (j = 0; j != new_width; j++) {
            x = j * x_ratio;
            y = i * y_ratio;

            pixels[i][j].red = img->pArr[(int)y][(int)x].red;
            pixels[i][j].green = img->pArr[(int)y][(int)x].green;
            pixels[i][j].blue = img->pArr[(int)y][(int)x].blue;
        }
    }

    for (int p = 0; p != image_get_height(img); p++) {
        free(img->pArr[p]);
        img->pArr[p] = NULL;
    }
    free(img->pArr);
    img->pArr = NULL;

    img->pArr = pixels;
    img->height = new_height;
    img->width = new_width;
}

void display_image(struct Pixel** pArr, int width, int height) {
    for (int i = 0; i != height; i++) {
        printf("ROW %d\t", i);
        for (int j = 0; j != width; j++) {
            printf("PIXEL: %d B:%d, G:%d, R:%d\t", j, pArr[i][j].blue, pArr[i][j].green, pArr[i][j].red);
        }
        printf("\n");
    }
}


int calculate_padding(int width) {
    int padding = 0;
    if (width % 4 != 0) {
        int next_divisible = ((width * 3) + 4) - ((width * 3) % 4);
        padding = next_divisible - (width * 3);
    }
    return padding;
}