/**
 * ImageProcessor Main file
 *
 * Completion time: 12h
 *
 * @author Claudio Rodriguez Rodriguez
 * @version 01/23/2022
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "BMPHandler.h"
#include "Image.h"

// PREPROCESSOR DEFINITIONS
#define DEBUG 0

// TYPE DEFINITIONS
typedef enum { false, true } bool;

struct Cli_Options {
    bool w;
    char *input_filename;
    char *r_value;
    char *g_value;
    char *b_value;
    char *s_value;
    char *output_file;
};

typedef struct Cli_Options Cli_Options;

struct ImageProcessorOptions {
    bool w;
    int r_value;
    int g_value;
    int b_value;
    float scaling_factor;
    bool scaling_flag;
    char input_file[256];
    char output_file[256];
};

typedef struct ImageProcessorOptions ImageProcessorOptions;

// FORWARD DECLARATIONS

// Creates Cli_Options
// returns ptr to object
Cli_Options* cli_options_create();

// Creates ImageProcessorOptions
// returns ptr to object
ImageProcessorOptions* image_processor_create(Cli_Options* cli_options);

// Destroys ImageProcessorOptions
//  @param  img: the image to destroy.
void image_processor_options_destroy(ImageProcessorOptions ** options);

// Shows user selections
void display_user_selections(ImageProcessorOptions* options);

// obtains cli options and adds them to struct
// @param argc comes from main
// @param argv comes from main
// @param cli_options a pointer to an empty Cli_Options struct
void get_cli_options(int argc, char **argv, Cli_Options *cli_options);

// Destroys Cli_Options
//  @param  options: options to destroy
void cli_options_destroy(Cli_Options ** options);

// Prints help message
void help();


// Main entry point of the program
// Accepts CLI arguments
// ImageProcessor <input_file> [options]
//   <input_file> must be a valid BMP file
//   options:
//     -w Applies a grayscale filter to image
//     -r <int> Color Shift +-255 values, anything
//          larger than 255 will be clamped respectively
//     -g <int> Color Shift +-255 values, anything
//          larger than 255 will be clamped respectively
//     -b <int> Color Shift +-255 values, anything
//          larger than 255 will be clamped respectively
//     -s <scaling_factor> Provide a float number
//          to scale the image up or down
//     -o <output_file> Output file to save the image
//          Otherwise output filename is <input_file>_copy.bmp
//     -h Shows this menu
int main(int argc, char **argv) {

    Cli_Options *cli_options = cli_options_create();
    FILE *file = NULL;
    BMP_Header bmp_header;
    DIB_Header dib_header;
    get_cli_options(argc, argv, cli_options);

    ImageProcessorOptions *userOptions = image_processor_create(cli_options);
    printf("User Selections.\n");
    display_user_selections(userOptions);

    cli_options_destroy(&cli_options);
    file = fopen(userOptions->input_file,"rb");
    if (!file)
    {
        printf("Cannot open file %s.\n", userOptions->input_file);
        return 1;
    }

    readBMPHeader(file, &bmp_header);
    readDIBHeader(file, &dib_header);

    if (DEBUG) {
        displayBMPHeader(&bmp_header);
        displayDIBHeader(&dib_header);
    }

    // unfortunately only support 24 bit images for now
    if (bmp_header.signature[0] != 'B' &&
        bmp_header.signature[1] != 'M' ||
        dib_header.width < 0 ||
        dib_header.height < 0 ||
        dib_header.bit_count != 24) {
        printf("Can not read image file %s.\n", userOptions->input_file);
        return 1;
    }

    Pixel** pixels = (Pixel**)malloc(sizeof(Pixel*) * dib_header.height);
    for (int p = 0; p != dib_header.height; p++) {
        pixels[p] = (Pixel*)malloc(sizeof(Pixel) * dib_header.width);
    }
    Image* img = image_create(pixels, dib_header.width, dib_header.height);
    readPixelsBMP(file, img->pArr, dib_header.width, dib_header.height);
    fclose(file);

    if (DEBUG) {
        display_image(pixels, dib_header.width, dib_header.height);
    }

    if (userOptions->w) {
        image_apply_bw(img);
    }

    image_apply_colorshift(img, userOptions->r_value, userOptions->g_value, userOptions->b_value);

    if (userOptions->scaling_flag) {
        image_apply_resize(img, userOptions->scaling_factor);
    }

    FILE* file_output = fopen(userOptions->output_file, "wb");
    BMP_Header *BMP = (BMP_Header*)malloc(sizeof(BMP_Header));
    DIB_Header *DIB = (DIB_Header*)malloc(sizeof(DIB_Header));
    makeBMPHeader(BMP, img->width, img->height);
    makeDIBHeader(DIB, img->width, img->height);
    writeBMPHeader(file_output, BMP);
	writeDIBHeader(file_output, DIB);
	writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_height(img));
    fclose(file_output);


    for (int p = 0; p != image_get_height(img); p++) {
        free(img->pArr[p]);
        img->pArr[p] = NULL;
    }
    free(img->pArr);
    img->pArr = NULL;

    free(BMP);
    free(DIB);
    image_destroy(&img);
    image_processor_options_destroy(&userOptions);
	
	return 0;
}

Cli_Options* cli_options_create() {
    Cli_Options *cli_options = (Cli_Options*) malloc(sizeof (Cli_Options));
    if (cli_options == NULL) {
        printf("Failed to create CLI Options.\n");
        exit(1);
    }
    cli_options->w = false;
    cli_options->input_filename = NULL;
    cli_options->r_value = NULL;
    cli_options->g_value = NULL;
    cli_options->b_value = NULL;
    cli_options->s_value = NULL;
    cli_options->output_file = NULL;
    return cli_options;
}

void cli_options_destroy(Cli_Options ** options) {
    free(*options);
    *options = NULL;
}

void help() {
    printf("\nImageProcessor <input_file> [options]\n\n");
    printf("\t<input_file> must be a valid BMP file\n");
    printf("\toptions:\n");
    printf("\t-w Applies a grayscale filter to image\n");
    printf("\t-r <int> Color Shift +-255 values\n");
    printf("\t-g <int> Color Shift +-255 values\n");
    printf("\t-b <int> Color Shift +-255 values\n");
    printf("\t-s <scaling_factor> Provide a float number to scale the image\n");
    printf("\t\tto scale the image up or down\n");
    printf("\t-o <output_file> Output file to save the image\n");
    printf("\t\tOtherwise output filename is <input_file>_copy.bmp\n");
    printf("\t-h Shows this menu\n");
}

void get_cli_options(int argc, char **argv, Cli_Options *cli_options) {
    int options = 0;
    while ((options = getopt (argc, argv, "wr:g:b:s:o:h")) != -1) {
        switch (options) {
            case 'w':
                cli_options->w = true;
                if (DEBUG)
                    printf("grayscale filter\n");
                break;
            case 'r':
                cli_options->r_value = optarg;
                if (DEBUG)
                    printf("red color shift %s\n", cli_options->r_value);
                break;
            case 'g':
                cli_options->g_value = optarg;
                if (DEBUG)
                    printf("green color shift %s\n", cli_options->g_value);
                break;
            case 'b':
                cli_options->b_value = optarg;
                if (DEBUG)
                    printf("blue color shift %s\n", cli_options->b_value);
                break;
            case 's':
                cli_options->s_value = optarg;
                if (DEBUG)
                    printf("scaling factor %s\n", cli_options->s_value);
                break;
            case 'o':
                cli_options->output_file = optarg;
                if (DEBUG)
                    printf("output_filename %s\n", cli_options->output_file);
                break;
            case '?':
                printf("Error: unknown filter -%c.\n", optopt);
                help();
                exit(1);
            case 'h':
            default:
                help();
                exit(0);
        }
    }

    cli_options->input_filename = argv[optind];
    if (cli_options->input_filename == NULL) {
        printf("Error: input_filename is required.\n");
        help();
        exit(1);
    }
    if (DEBUG)
        printf("input_filename %s\n", cli_options->input_filename);

    if (access(cli_options->input_filename, F_OK) != 0) {
        printf("Error: input_filename %s does not exist.\n", cli_options->input_filename);
        exit(1);
    }

    if (access(cli_options->input_filename, R_OK) != 0) {
        printf("Error: can't read input_filename %s.\n", cli_options->input_filename);
        exit(1);
    }
}

void display_user_selections(ImageProcessorOptions* options) {
    if (options->g_value != 0) {
        printf("Color Shift Green by %d\n", options->g_value);
    }
    if (options->r_value != 0) {
        printf("Color Shift Red by %d\n", options->r_value);
    }
    if (options->b_value != 0) {
        printf("Color Shift Blue by %d\n", options->b_value);
    }
    if (options->scaling_flag) {
        printf("Scale Image by %f\n", options->scaling_factor);
    }
    if (options->w) {
        printf("Apply Grayscale\n");
    }
    if (strlen(options->output_file) != 0) {
        printf("Output file %s\n", options->output_file);
    }
}

ImageProcessorOptions* image_processor_create(Cli_Options* cli_options) {
    ImageProcessorOptions *image_options = (ImageProcessorOptions*) malloc(sizeof (ImageProcessorOptions));
    if (image_options == NULL) {
        printf("Failed to create Image Options.\n");
        exit(1);
    }
    image_options->g_value = 0;
    image_options->r_value = 0;
    image_options->b_value = 0;
    image_options->scaling_flag = false;
    image_options->w = cli_options->w;

    if (cli_options->g_value != NULL) {
        image_options->g_value = (int) strtol(cli_options->g_value, (char **)NULL, 10);
        if (image_options->g_value > 255) {
            image_options->g_value = 255;
        } else if (image_options->g_value < -255) {
            image_options->g_value = -255;
        }
    }
    if (cli_options->r_value != NULL) {
        image_options->r_value = (int) strtol(cli_options->r_value, (char **)NULL, 10);
        if (image_options->r_value > 255) {
            image_options->r_value = 255;
        } else if (image_options->r_value < -255) {
            image_options->r_value = -255;
        }
    }
    if (cli_options->b_value != NULL) {
        image_options->b_value = (int) strtol(cli_options->b_value, (char **)NULL, 10);
        if (image_options->b_value > 255) {
            image_options->b_value = 255;
        } else if (image_options->b_value < -255) {
            image_options->b_value = -255;
        }
    }
    if (cli_options->s_value != NULL) {
        image_options->scaling_flag = true;
        image_options->scaling_factor = (float) strtof(cli_options->s_value, NULL);
    }
    strcpy(image_options->input_file, cli_options->input_filename);
    if (cli_options->output_file) {
        strcpy(image_options->output_file, cli_options->output_file);
    } else {

        int n = 0;
        int len = strlen(cli_options->input_filename);
        for (int i = len; i > -1; i--) {
            if (cli_options->input_filename[i] == '.') {
                n = i;
                break;
            }
        }

        if (n != 0) {
            char trimmed[256];
            memcpy(trimmed, cli_options->input_filename, n);
            strcpy(image_options->output_file, strcat(trimmed, "_copy.bmp"));
        } else {
            strcpy(image_options->output_file, strcat(cli_options->input_filename, "_copy.bmp"));
        }
    }
    return image_options;
}

void image_processor_options_destroy(ImageProcessorOptions ** options) {
    free(*options);
    *options = NULL;
}