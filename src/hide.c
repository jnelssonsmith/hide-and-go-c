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

/*
To ensure the program exits gracefully (doesn't leave half written to files or leave things open) 
we define a custom exit gracefully function. It is useful because if for some reason the program fails 
during some step of execution with the output file being created, we will be left with a half written to 
file. This function attempts to cover up any of our evil deeds should they fail, otherwise it does the busy work
of ensuring files are closed and the correct exit signal is broadcast. 
*/
void exitGracefully(int error, char fileName[], FILE *inputFP, FILE *outputFP);

int main(int argc, char **argv) {
	
	FILE *inputFP,	// the file pointer to the input ppm image
		 *outputFP; // the file pointer to the output ppm image with the hidden message inside
	
	int temp, 					  // used for processing chars
		height,					  // the read height of the ppm image
		width,					  // the read width of the ppm image
		colourRange,			  // the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				  // an error storing variable, 0 on error, 1 on no error

	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
		exitGracefully(EXIT_FAILURE, "", NULL, NULL);
	} else {
		inputFP = fopen(argv[1], "rb");
		if (inputFP == NULL) {
			fprintf(stderr, "Could not open supplied file: %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		outputFP = fopen(argv[2], "wb");

		if(isRawPPM(inputFP, outputFP)){
			scanToNextVal(inputFP, outputFP);
			
			width = getWidth(inputFP);
			fprintf(outputFP, "%d", width);

			scanToNextVal(inputFP, outputFP);

			height = getHeight(inputFP);
			fprintf(outputFP, "%d", height);

			scanToNextVal(inputFP, outputFP);

			colourRange = getColourRange(inputFP);
			fprintf(outputFP, "%d", colourRange);
			temp = fgetc(inputFP); // just get one white space character
			fputc(temp, outputFP);
			
			maxSizeSupportedByImage = getSupportedImageBytes(width, height);
			error = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);
			exitGracefully(error, argv[2], inputFP, outputFP);
			
		} else {
			printf("Incorrect file format detected, aborting\n");
			exitGracefully(EXIT_FAILURE, argv[2], inputFP, outputFP);
		}

		
	}
}

void exitGracefully(int error, char fileName[], FILE *inputFP, FILE *outputFP) {
	if(inputFP != NULL){
		fclose(inputFP);
	}

	if (outputFP != NULL) {
		fclose(outputFP);
	}

	if(!error) { 
		exit(EXIT_SUCCESS);
	} else {
		exit(EXIT_FAILURE);
		if(fileName != "") {
			remove(fileName);
		}
		
	}
}