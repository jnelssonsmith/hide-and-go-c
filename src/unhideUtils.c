#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages

void unhideMessage(FILE *inputFP, int multiMode) {
	int size; // used to store read in size of secret message

	if(multiMode) {
		
	}
	// get the size out of the image data
	size = readSizeOfSecretMessage(inputFP);

	if(size == 0) {
		/* if the size is 0, we know there is no point wasting time reading the secret message so we can exit
		   we also know this is a safe way to do things for the -m flag because a 0 byte size implies 
		   that any following files will also have a 0 byte size 											  */
		exitGracefully(0, "", inputFP, NULL);
	}

	// then use the size to read in the secret message and output it to standard output
	readSecretMessage(size, inputFP);
}


FILE* getUnhideFP(char *inputName, int multiMode) {
	FILE *inputFP;	// the file pointer to the input ppm image

	int error; 		// an error storing variable, 1 on error, 0 on no error, 

    inputFP = fopen(inputName, "r");


	// kill program if input file does not exist
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", inputName);
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

	return inputFP;
}

int getNumberOfFiles(FILE *inputFP) {
	int currentNum, // the current r,g or b val we have read from the image		
	    lastBit,    // the last bit of the current num 
		size = 0;   // the size of the secret message, as we read we increment this until we reach the size. 

	for(int i=7; i > -1; i--) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		fprintf(stderr, "%d", lastBit);
		size += lastBit << i;
	}

	return size;
}