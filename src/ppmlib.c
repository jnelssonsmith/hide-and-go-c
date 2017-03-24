 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 21/03/17
DESCRIPTION: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ppmlib.h"

int isRawPPM(FILE *inputFile, FILE *outputFile) {
	scanToNextVal(inputFile, outputFile);
	if (fgetc(inputFile) == 'P') {
		if(fgetc(inputFile) == '6') {
			fprintf(stderr, "File type is raw PPM (P6)\n");
			if(outputFile != NULL) {
				fputc('P', outputFile);
				fputc('6', outputFile);
			}
			return 1;
		}
	}

	return 0;
}

int getWidth(FILE *inputFP) {
	int currentChar;
	int width = 0; 
	while(1) {
		currentChar = fgetc(inputFP);
		if(isspace(currentChar)){
			ungetc(currentChar, inputFP);
			fprintf(stderr, "Width: %d\n", width);
			return width;
		} else {
			width = width * 10 + (currentChar - 48);
		}
	}
}

int getHeight(FILE *inputFP) {
	int currentChar;
	int height = 0; 
	while(1) {
		currentChar = fgetc(inputFP);
		if(isspace(currentChar)){
			ungetc(currentChar, inputFP);
			fprintf(stderr,"Height: %d\n", height);
			return height;
		} else {
			height = height * 10 + (currentChar - 48);
		}
	}
}

int getColourRange(FILE *inputFP) {
	int currentChar;
	int colourRange = 0; 
	while(1) {
		
		currentChar = fgetc(inputFP);
		if(isspace(currentChar)){
			if(currentChar < 1) {
				fprintf(stderr, "Colour range value must be greater than 0, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else if (currentChar > 65535) {
				fprintf(stderr, "Colour range value must be less than 65546, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else {
				fprintf(stderr, "Colour range: 0 to %d\n", colourRange);
				ungetc(currentChar, inputFP); //give back last grabbed value
				return colourRange;
			}
		} else {
			colourRange = colourRange * 10 + (currentChar - 48);
		}
	}
}

void scanToNextVal(FILE *inputFile, FILE *outputFile) {
	int temp;
	while(1){
		temp = fgetc(inputFile);
		if(isspace(temp)) {
			if(outputFile != NULL) {
				fputc(temp, outputFile);
			}
		} else if(temp == '#') {
			temp = fgetc(inputFile);
			while(temp != '\n') {
				temp = fgetc(inputFile);
			}
		} else {
			break;
		}
	} 
	ungetc(temp, inputFile);
}


void scanToImageData(FILE *inputFP) {
	scanToNextVal(inputFP, NULL);
	getWidth(inputFP);
	scanToNextVal(inputFP, NULL);
	getHeight(inputFP);
	scanToNextVal(inputFP, NULL);
    getColourRange(inputFP);
	scanToNextVal(inputFP, NULL);
}