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
#include <unistd.h>
#include <string.h>

#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages
#include "hideUtils.h" 

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

int argParse(int argc, char *argv[]) {
	if (argc >= 3) {
		if (strcmp("-m", argv[1]) == 0) {
			// expecting input of form "$ hide -m numberOfFiles inputBasename outputBasename"
			if(argc == 5) {
				return 'm'; // process file flag
			} 
		
		} else if (strcmp("-p", argv[1]) == 0) {
			// expecting input of form "$ hide -p instructionFile"
			if(argc == 3) {
				return 'p'; // process file flag
			} 
		
		} else if (strcmp("-s", argv[1]) == 0) {
			// expecting input of form "$ hide -s inputPPM outputPPM"
			if(argc == 4) {
				return 's'; // side by side flag
			}

		} else {
			// have to assume standard hide at this stage of form "$ hide inputPPM outputPPM"
			if(argc == 3) {
				return '0'; // default case flag
			}
		}

		return EXIT_FAILURE;

	} else {
		return EXIT_FAILURE;
	}
}

int main(int argc, char *argv[]) {
	
	FILE *inputFP,	// the file pointer to the input ppm image
		 	 *outputFP; // the file pointer to the output ppm image with the hidden message inside
	
	
	int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error,				      			// an error storing variable, 1 on error, 0 on no error
		flagType;
	
	flagType = argParse(argc, argv);

	switch(flagType) {
		case 's':
			fprintf(stderr, "s flag detected\n");
			break;
		case 'p':
			fprintf(stderr, "p flag detected\n");
			break;
		case 'm':
			fprintf(stderr, "m flag detected\n");
			break;
		case '0':
			fprintf(stderr, "no flags detected\n");
			standardHideMessage(argv[1], argv[2]);
			break;
	
		default:
			printf("Incorrect arg format detected\nHide only supports 1 flag at a time or no flags\n");
			exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

