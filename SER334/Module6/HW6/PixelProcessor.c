#include "PixelProcessor.h"

void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift, int leave_black) {
    for (int i = 0; i != height; i++) {
        for (int j = 0; j != width; j++) {
            if (leave_black) {
                if (pArr[i][j].red == 0 && pArr[i][j].green == 0 && pArr[i][j].blue == 0) {
                    continue;
                }
            }
            if (rShift != 0) {
                if (pArr[i][j].red + rShift > 255) {
                    pArr[i][j].red = 255;
                } else if (pArr[i][j].red + rShift < 0) {
                    pArr[i][j].red = 0;
                } else {
                    pArr[i][j].red += rShift;
                }
            }
            if (gShift != 0) {
                if (pArr[i][j].green + gShift > 255) {
                    pArr[i][j].green = 255;
                } else if (pArr[i][j].green + gShift < 0) {
                    pArr[i][j].red = 0;
                } else {
                    pArr[i][j].green += gShift;
                }
            }
            if (bShift != 0) {
                if (pArr[i][j].blue + bShift > 255) {
                    pArr[i][j].blue = 255;
                } else if (pArr[i][j].blue + bShift < 0) {
                    pArr[i][j].blue = 0;
                } else {
                    pArr[i][j].blue += bShift;
                }
            }
        }
    }
}