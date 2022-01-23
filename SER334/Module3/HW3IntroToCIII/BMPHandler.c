
#include "BMPHandler.h"


void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fseek(file, 0, SEEK_SET);
    fread(&header->signature,            sizeof(char) * 2, 1, file);
    fread(&header->size,                 sizeof(uint32_t), 1, file);
    fread(&header->reserved1,            sizeof(uint16_t), 1, file);
    fread(&header->reserved2,            sizeof(uint16_t), 1, file);
    fread(&header->offset_pixel_array,   sizeof(uint32_t), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header){
    fwrite(&header->signature,            sizeof(char) * 2, 1, file);
    fwrite(&header->size,                 sizeof(uint32_t), 1, file);
    fwrite(&header->reserved1,            sizeof(uint16_t), 1, file);
    fwrite(&header->reserved2,            sizeof(uint16_t), 1, file);
    fwrite(&header->offset_pixel_array,   sizeof(uint32_t), 1, file);
}

void readDIBHeader(FILE* file, struct DIB_Header* header){
    fread(&header->size_dib,        sizeof(uint32_t), 1, file);
    fread(&header->width,           sizeof(int32_t), 1, file);
    fread(&header->height,          sizeof(int32_t), 1, file);
    fread(&header->planes,          sizeof(uint16_t), 1, file);
    fread(&header->bit_count,       sizeof(uint16_t), 1, file);
    fread(&header->bi_rgb,          sizeof(uint32_t), 1, file);
    fread(&header->size_image,      sizeof(uint32_t), 1, file);
    fread(&header->x_print_ppm,     sizeof(int32_t), 1, file);
    fread(&header->y_print_ppm,     sizeof(int32_t), 1, file);
    fread(&header->clr_palette,     sizeof(uint32_t), 1, file);
    fread(&header->clr_important,   sizeof(uint32_t), 1, file);
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->size_dib,        sizeof(uint32_t), 1, file);
    fwrite(&header->width,           sizeof(int32_t), 1, file);
    fwrite(&header->height,          sizeof(int32_t), 1, file);
    fwrite(&header->planes,          sizeof(uint16_t), 1, file);
    fwrite(&header->bit_count,       sizeof(uint16_t), 1, file);
    fwrite(&header->bi_rgb,          sizeof(uint32_t), 1, file);
    fwrite(&header->size_image,      sizeof(uint32_t), 1, file);
    fwrite(&header->x_print_ppm,     sizeof(int32_t), 1, file);
    fwrite(&header->y_print_ppm,     sizeof(int32_t), 1, file);
    fwrite(&header->clr_palette,     sizeof(uint32_t), 1, file);
    fwrite(&header->clr_important,   sizeof(uint32_t), 1, file);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height){
    int padding = calculate_padding(width);
    int bytes_per_row = (width * 3) + padding;

    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = sizeof(BMP_Header) + sizeof(DIB_Header) - 2;
    header->size = header->offset_pixel_array + bytes_per_row * height;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    int padding = calculate_padding(width);
    int bytes_per_row = (width * 3) + padding;

    header->size_dib = 40;
    header->width = width;
    header->height = height;
    header->planes = 1;
    header->bit_count = 24;
    header->bi_rgb = 0;
    header->size_image = bytes_per_row * height;
    header->x_print_ppm = 0;
    header->y_print_ppm = 0;
    header->clr_important = 0;
    header->clr_palette = 0;
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    int i = 0;
    int j = 0;

    int padding = calculate_padding(width);
    for(i = height - 1; i >= 0; i--){
        for(j = 0; j != width; j++){
            fread(&pArr[i][j].blue, sizeof(char), 1, file);
            fread(&pArr[i][j].green, sizeof(char), 1, file);
            fread(&pArr[i][j].red, sizeof(char), 1, file);
        }
        if (padding != 0) {
            fseek(file, sizeof(unsigned char) * padding, SEEK_CUR);
        }
    }
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    int i = 0;
    int j = 0;
    int k = 0;
    int padding = calculate_padding(width);
    for(i = height - 1; i >= 0; i--){
        for(j = 0; j != width; j++){
            fwrite(&pArr[i][j].blue, sizeof(char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(char), 1, file);
            fwrite(&pArr[i][j].red, sizeof(char), 1, file);
        }
        if (padding != 0) {
            for (k = 0; k < padding; k++) {
                fputc(0, file);
            }
        }
    }
}

void displayDIBHeader(struct DIB_Header* header) {
    printf("size_dib: %d\n", header->size_dib);
    printf("width: %d\n", header->width);
    printf("height: %d\n", header->height);
    printf("planes: %d\n", header->planes);
    printf("bit_count: %d\n", header->bit_count);
    printf("bi_rgb: %d\n", header->bi_rgb);
    printf("size_image: %d\n", header->size_image);
    printf("x_print_ppm: %d\n", header->x_print_ppm);
    printf("y_print_ppm: %d\n", header->y_print_ppm);
    printf("clr_palette: %d\n", header->clr_palette);
    printf("clr_important: %d\n", header->clr_important);
}

void displayBMPHeader(struct BMP_Header* header) {
    printf("signature: %c%c\n", header->signature[0], header->signature[1]);
    printf("size: %d\n", header->size);
    printf("reserved1: %d\n", header->reserved1);
    printf("reserved2: %d\n", header->reserved2);
    printf("offset_pixel_array: %d\n", header->offset_pixel_array);
}
