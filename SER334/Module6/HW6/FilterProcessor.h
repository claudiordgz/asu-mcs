/**
* A utility functions for doing pixel transformations.
*
* Completion time: -1 minutes
*
* @author Claudio Rodriguez Rodriguez
* @version 02/07/2022
*/

#ifndef FILTER_PROCESSOR_H
#define FILTER_PROCESSOR_H

#include "PixelProcessor.h"

/**
 * Applies Box Blur Filter on an array of Pixels
 *
 * @param  pArr: Pixel array of the image to write to the file
 * @param  threads: Number of threads to use
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void blur_filter(struct Pixel** pArr, int threads, int width, int height);

/**
 * Applies Cheese Filter on an array of Pixels
 *
 * @param  pArr: Pixel array of the image to write to the file
 * @param  threads: Number of threads to use
 * @param  width: Width of the pixel array of this image
 * @param  height: Height of the pixel array of this image
 */
void cheese_filter(struct Pixel** pArr, int threads, int width, int height);

#endif