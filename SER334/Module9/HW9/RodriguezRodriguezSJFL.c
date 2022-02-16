


void get_cli_options(int argc, char **argv, Cli_Options *cli_options);

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
  FILE *file = NULL;
  return 0;
}