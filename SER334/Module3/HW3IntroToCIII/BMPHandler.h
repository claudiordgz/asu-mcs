/**
* Implementation of several functions to manipulate BMP files.
*
* Completion time: 4h
*
* @author Claudio Rodriguez Rodriguez, Ruben Acuna
* @version 01/23/2022
*/

#include <stdio.h>
#include <stdint.h>
#include "Image.h"

struct BMP_Header {
    char       signature[2];       // ID field "BM"
    uint32_t   size;               // Size of the BMP file
    uint16_t   reserved1;          // Application specific
    uint16_t   reserved2;          // Application specific
    uint32_t   offset_pixel_array; // Offset where the pixel array can be found
};

typedef struct BMP_Header BMP_Header;

struct DIB_Header {
    uint32_t size_dib;        // Size of DIB header
    int32_t  width;           // width of the bitmap in pixels
    int32_t  height;          // height of the bitmap in pixels
    uint16_t planes;          // number of color planes
    uint16_t bit_count;       // number of bits per pixel
    uint32_t bi_rgb;          // BI_RGB, 0 if no compression
    uint32_t size_image;      // size of the raw bitmap data (with padding)
    int32_t  x_print_ppm;     // print resolution of the image (pixels per meter)
    int32_t  y_print_ppm;     //
    uint32_t clr_palette;     // number of colors in the palette
    uint32_t clr_important;   // 0 means all colors are important
};

typedef struct DIB_Header DIB_Header;

/**
 * Read BMP header of a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * Write BMP header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header);

/**
 * Read DIB header from a BMP file.
 *
 * @param  file: A pointer to the file being read
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * Write DIB header of a file. Useful for creating a BMP file.
 *
 * @param  file: A pointer to the file being written
 * @param  header: The header to write to the file
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header);

/**
 * Make BMP header based on width and height. Useful for creating a BMP file.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void makeBMPHeader(struct BMP_Header* header, int width, int height);

/**
* Make new DIB header based on width and height.Useful for creating a BMP file.
*
* @param  header: Pointer to the destination DIB header
* @param  width: Width of the image that this header is for
* @param  height: Height of the image that this header is for
*/
void makeDIBHeader(struct DIB_Header* header, int width, int height);

/**
 * Read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read
 * @param  pArr: Pixel array to store the pixels being read
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);

/**
 * Write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image to write to the file
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);

// Prints the DIB header to the screen
void displayDIBHeader(struct DIB_Header* header);

// Prints the BMP header to the screen
void displayBMPHeader(struct BMP_Header* header);