#include <stdio.h>
#include <stdlib.h> 
#include <argp.h>
#include <unistd.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages

struct arguments {
  int numberOfFiles;      		  /* Argument for -m */
  char *instructionFile;    		/* Argument for -p */
  int sideBySide; 							/* Indicates precence of -s flag */
  char *inputPPM;
  char *outputPPM;
};

void standardHideMessage(struct arguments arguments) {
    FILE *inputFP,	// the file pointer to the input ppm image
	     *outputFP; // the file pointer to the output ppm image with the hidden message inside

    int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				 

    inputFP = fopen(arguments.inputPPM, "rb");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", arguments.inputPPM);
		exit(EXIT_FAILURE);
    }
	
	outputFP = fopen(arguments.outputPPM, "wb");
	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, outputFP);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, arguments.outputPPM, inputFP, outputFP);
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
		exitGracefully(PPM_READ_ERROR, arguments.outputPPM, inputFP, outputFP);
	}
	
    fprintf(outputFP, "%d", colourRange);
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 
	fputc(temp, outputFP);
	
	maxSizeSupportedByImage = getSupportedImageBytes(width, height);
	/* now we can hide the message, the fail state is caught in the error variable, regardless of if the hide 
		is successful or not we exit gracefully because it is the last thing we need to do */
	error = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);
	
    exitGracefully(error, arguments.outputPPM, inputFP, outputFP);
}