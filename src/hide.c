 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 25/03/17
DESCRIPTION: hide is a program that reads in a ppm image as it's first argument 
and an output file name as it's second argument. It then takes in a message to 
hide within the ppm image through stdin. If there are no errors with the 
program, hide will hide the stdin message inside the given ppm image and 
output it as a file with the name given by the second argument.
*/

#include <stdio.h>
#include <stdlib.h> 
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages


/* 
These are usually defined but just in case the test computers use some weird setup we 
define the success and failure variables for easier readability of the code and what 
constitutes a failed and a successful finish of execution
*/
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE	1
#endif


int main(int argc, char **argv) {
	
	FILE *inputFP,	// the file pointer to the input ppm image
		 *outputFP; // the file pointer to the output ppm image with the hidden message inside
	
	int temp, 					  // used for processing chars
		height,					  // the read height of the ppm image
		width,					  // the read width of the ppm image
		colourRange,			  // the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				      // an error storing variable, 1 on error, 0 on no error
	

	// we kill the program if an incorrect number of args is provided
	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
		exit(EXIT_FAILURE);
	}

	inputFP = fopen(argv[1], "rb");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	outputFP = fopen(argv[2], "wb");
	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, outputFP);
	if(error) {
		printf("Incorrect file format detected, aborting\n");
		exitGracefully(error, argv[2], inputFP, outputFP);
	}
	
	// scan through width, heigh and colour range values
	scanToNextVal(inputFP, outputFP);
	
	width = getWidth(inputFP);
	fprintf(outputFP, "%d", width);
	
	scanToNextVal(inputFP, outputFP);
	
	height = getHeight(inputFP);
	fprintf(outputFP, "%d", height);

	scanToNextVal(inputFP, outputFP);
	
	colourRange = getColourRange(inputFP);
	if(colourRange == PPM_COLOUR_READ_ERROR) {
		exitGracefully(PPM_READ_ERROR, argv[2], inputFP, outputFP);
	}
	fprintf(outputFP, "%d", colourRange);
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 
	fputc(temp, outputFP);
	

	maxSizeSupportedByImage = getSupportedImageBytes(width, height);


	/* now we can hide the message, the fail state is caught in the error variable, regardless of if the hide 
	   is successful or not we exit gracefully because it is the last thing we need to do */
	error = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);

	exitGracefully(error, argv[2], inputFP, outputFP);
}

