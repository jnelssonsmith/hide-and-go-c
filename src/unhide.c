 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 18/03/17
DESCRIPTION: unhide is a program that reads in a ppm image with a hidden message 
and returns the hidden message to standard output. 
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
	
	FILE *inputFP;	// the file pointer to the input ppm image

	int size,		// used to store read in size of secret message
		error; 		// an error storing variable, 1 on error, 0 on no error, 

	// kill program if incorrect number of args is supplied
	if(argc != 2) {
		fprintf(stderr, "Incorrect number of arguments supplied, unhide expects 1 argument\n[1] path to a ppm file with a hidden message in it\n");
		exit(EXIT_FAILURE);
	}

	inputFP = fopen(argv[1], "rb");

	// kill program if input file does not exist
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	
	// check input is raw ppm format
	error = isRawPPM(inputFP, NULL);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, "", inputFP, NULL);
	}
	
	// skip to the image data, which is all we care about when reading a secret message
	scanToImageData(inputFP);

	// get the size out of the image data
	size = readSizeOfSecretMessage(inputFP);

	// then use the size to read in the secret message and output it to standard output
	readSecretMessage(size, inputFP);

	// once read we can close everything up
	exitGracefully(EXIT_SUCCESS, "", inputFP, NULL);
}


