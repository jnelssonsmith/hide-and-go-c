#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages

void unhideMessage(char *inputName, int multiMode) {
    FILE *inputFP;	// the file pointer to the input ppm image

	int size,		// used to store read in size of secret message
		error; 		// an error storing variable, 1 on error, 0 on no error, 

    inputFP = fopen(inputName, "r");

	// kill program if input file does not exist
	if (inputFP == NULL) {
		if(multiMode) {
			exitGracefully(EXIT_SUCCESS, "", NULL, NULL);
		} else {
			fprintf(stderr, "Could not open supplied file: %s\n", inputName);
			exit(EXIT_FAILURE);
		}
		
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
}