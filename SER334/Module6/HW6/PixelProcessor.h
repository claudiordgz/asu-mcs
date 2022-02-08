/**
* A utility functions for doing pixel transformations.
*
* Completion time: 10 minutes
*
* @author Claudio Rodriguez Rodriguez, Vatrcia Edgar
* @version 02/07/2022
*/

#ifndef PixelProcessor_H
#define PixelProcessor_H

struct Pixel{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

//NOT NEEDED FOR THREADING HW.
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift, int leave_black);
#endif