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
#include <pthread.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

// PREPROCESSOR DEFINITIONS
#define DEBUG 1

// TYPE DEFINITIONS
typedef enum { blur, cheese } filter_type;
typedef struct Pixel Pixel;
typedef struct BMP_Header BMP_Header;
typedef struct DIB_Header DIB_Header;

struct Cli_Options {
    char *input_filename;
    char *output_file;
    char *filter_type;
};

typedef struct Cli_Options Cli_Options;

struct ImageProcessorOptions {
    filter_type filter;
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
// Filters -i <input_file> [options]
// -i <input_file> must be a valid BMP file
// options:
// -o <output_file> Output file to save the image
// 	Otherwise output filename is <input_file>_copy.bmp
// -f Filter to apply 
// 	Valid Filters:
// 	b - Blur box Filter
// 	c - Cheese Filter
// -h Shows this menu
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

    // unfortunately only support 24 bit images for now
    if (bmp_header.signature[0] != 'B' &&
        bmp_header.signature[1] != 'M' ||
        dib_header.width < 0 ||
        dib_header.height < 0 ||
        dib_header.bitsPerPixel != 24) {
        printf("Can not read image file %s.\n", userOptions->input_file);
        return 1;
    }

    Pixel** pixels = (Pixel**)malloc(sizeof(Pixel*) * dib_header.height);
    for (int p = 0; p != dib_header.height; p++) {
        pixels[p] = (Pixel*)malloc(sizeof(Pixel) * dib_header.width);
    }
    readPixelsBMP(file, pixels, dib_header.width, dib_header.height);
    fclose(file);

    // if (DEBUG) {
    //     display_image(pixels, dib_header.width, dib_header.height);
    // }

    // if (userOptions->w) {
    //     image_apply_bw(img);
    // }

    // image_apply_colorshift(img, userOptions->r_value, userOptions->g_value, userOptions->b_value);

    // if (userOptions->scaling_flag) {
    //     image_apply_resize(img, userOptions->scaling_factor);
    // }

    // FILE* file_output = fopen(userOptions->output_file, "wb");
    // BMP_Header *BMP = (BMP_Header*)malloc(sizeof(BMP_Header));
    // DIB_Header *DIB = (DIB_Header*)malloc(sizeof(DIB_Header));
    // makeBMPHeader(BMP, img->width, img->height);
    // makeDIBHeader(DIB, img->width, img->height);
    // writeBMPHeader(file_output, BMP);
	// writeDIBHeader(file_output, DIB);
	// writePixelsBMP(file_output, image_get_pixels(img), image_get_width(img), image_get_height(img));
    // fclose(file_output);


    // for (int p = 0; p != image_get_height(img); p++) {
    //     free(img->pArr[p]);
    //     img->pArr[p] = NULL;
    // }
    // free(img->pArr);
    // img->pArr = NULL;

    // free(BMP);
    // free(DIB);
    // image_destroy(&img);
    // image_processor_options_destroy(&userOptions);
	
	return 0;
}

Cli_Options* cli_options_create() {
    Cli_Options *cli_options = (Cli_Options*) malloc(sizeof (Cli_Options));
    if (cli_options == NULL) {
        printf("Failed to create CLI Options.\n");
        exit(1);
    }
    cli_options->filter_type = NULL;
    cli_options->input_filename = NULL;
    cli_options->output_file = NULL;
    return cli_options;
}

void cli_options_destroy(Cli_Options ** options) {
    free(*options);
    *options = NULL;
}

void help() {
    printf("\nFilters -i <input_file> [options]\n\n");
    printf("\t-i <input_file> must be a valid BMP file\n");
    printf("\toptions:\n");
    printf("\t-o <output_file> Output file to save the image\n");
    printf("\t\tOtherwise output filename is <input_file>_copy.bmp\n");
    printf("\t-f Filter to apply \n");
    printf("\t\tValid Filters:\n");
    printf("\t\tb - Blur box Filter\n");
    printf("\t\tc - Cheese Filter\n");
    printf("\t-h Shows this menu\n");
}

void get_cli_options(int argc, char **argv, Cli_Options *cli_options) {
    int options = 0;
    while ((options = getopt (argc, argv, "i:o:f:h")) != -1) {
        switch (options) {
            case 'i':
                cli_options->input_filename = optarg;
                if (DEBUG)
                    printf("input_filename %s\n", cli_options->input_filename);
                break;
            case 'f':
                cli_options->filter_type = optarg;
                if (DEBUG)
                    printf("filter type %s\n", cli_options->filter_type);
                if (strcmp(cli_options->filter_type, "b")   != 0 &&
                    strcmp(cli_options->filter_type, "c")   != 0 ) {
                        printf("Error: unknown filter -f %s.\n", cli_options->filter_type);
                        help();
                        exit(1);
                    }
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

    if (cli_options->input_filename == NULL) {
        printf("Error: input_filename is required.\n");
        help();
        exit(1);
    }

    if (cli_options->filter_type == NULL) {
        printf("Error: filter -f is required.\n");
        help();
        exit(1);
    }

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
    printf("Filter %d\n", options->filter);
    if (strlen(options->input_file) != 0) {
        printf("Input file %s\n", options->input_file);
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

    if (cli_options->filter_type != NULL) {
        image_options->filter = (filter_type) strtol(cli_options->filter_type, (char **)NULL, 10);
        if (strcmp(cli_options->filter_type, "b") == 0) {
            image_options->filter = blur;
        }
        else if (strcmp(cli_options->filter_type, "c") == 0) {
            image_options->filter = cheese;
        }
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