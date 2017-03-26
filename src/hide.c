 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 25/03/17
DESCRIPTION: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ppmlib.h"
#include "steglib.h"

void exitGracefully(int noError, char fileName[]);

int main(int argc, char **argv) {
	
	FILE *inputFP;
	FILE *outputFP;
	
	char message[65536];
	int messageSize;
	int height;
	int width;
	int colourRange;
	int scanCount;
	int maxSizeSupportedByImage;

	int byteIndex = 7;
	int *byteIndexPtr = &byteIndex;

	
	
	
	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
		exit(EXIT_FAILURE);
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
			int temp = fgetc(inputFP); // just get one white space character
			fputc(temp, outputFP);
			
			maxSizeSupportedByImage = getSupportedImageBytes(width, height);
			int noError = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);
			exitGracefully(noError, argv[2]);
			
		} else {
			printf("Incorrect file format detected, aborting\n");
			exitGracefully(0, argv[2]);
		}

		
	}
}



void exitGracefully(int noError, char fileName[]) {
	if(noError) { 
		exit(EXIT_SUCCESS);
	} else {
		remove(fileName);
		exit(EXIT_FAILURE);
	}
}