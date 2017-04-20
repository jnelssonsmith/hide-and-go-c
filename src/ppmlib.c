 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 26/03/17
DESCRIPTION: Here we implement the ppm library functions defined in ppmlib.h. 
Should you want more detailed information about the inputs, outputs and reasons for these 
functions please look at the header file. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ppmlib.h"

// raw ppm dictated by format specifier 'P6'
int isRawPPM(FILE *inputFile, FILE *outputFile) {
	scanToNextVal(inputFile, outputFile); //scan through any comments before the ppm value 
	if (fgetc(inputFile) == 'P') {
		if(fgetc(inputFile) == '6') {
			fprintf(stderr, "File type is raw PPM (P6)\n");
			if(outputFile != NULL) {
				fputc('P', outputFile);
				fputc('6', outputFile);
			}
			return PPM_READ_SUCCESS;
		}
	}

	return PPM_READ_ERROR;
}


int getWidth(FILE *inputFP) {
	int width;
	fscanf(inputFP, "%d", &width);
	fprintf(stderr, "Width: %d\n", width);
	return width;
}

int getHeight(FILE *inputFP) {
	int height;
	fscanf(inputFP, "%d", &height);
	fprintf(stderr, "Height: %d\n", height);
	return height;
}

int getColourRange(FILE *inputFP) {
	int colourRange;
	fscanf(inputFP, "%d", &colourRange);
	
	/* once we have the colour range we need to sanity check it, it should be between 1 and 255 inclusive, 
	   seeing as a colour range of 0 doesnt make sense */
	if(colourRange < 1) {
		fprintf(stderr, "Colour range value must be greater than 0, detected: %d", colourRange);
		return PPM_COLOUR_READ_ERROR;
	} else if (colourRange > 255) {
		fprintf(stderr, "Colour range value must be less than 256, detected: %d", colourRange);
		return PPM_COLOUR_READ_ERROR;
	} else {
		// colour range is correct so we return it
		fprintf(stderr, "Colour range: 0 to %d\n", colourRange);
		return colourRange;
	}
}


void scanToNextVal(FILE *inputFile, FILE *outputFile) {
	int temp;
	while(1){
		temp = fgetc(inputFile);
		if(isspace(temp)) { // we want to skip all white space characters
			if(outputFile != NULL) {
				fputc(temp, outputFile);
			}
		} else if(temp == '#') { // we skip comments until we hit a new line
			if(outputFile != NULL) {
				fputc(temp, outputFile);
			}
			temp = fgetc(inputFile);
			if(outputFile != NULL) {
				fputc(temp, outputFile);
			}
			while(temp != '\n') { // we can get out of this loop by hitting a new line
				temp = fgetc(inputFile);
				if(outputFile != NULL) {
					fputc(temp, outputFile);
				}
			}
		} else {
			break; // we only break once we are past all the comments and white space characters
		}
	} 
	// seeing as we do evaluation in the loop we would have grabbed one too many characters, so put it back
	ungetc(temp, inputFile); 
}


void scanToImageData(FILE *inputFP) {
	// all the get value functions return the values, but we do not care in this case so we just let them go
	scanToNextVal(inputFP, NULL);
	getWidth(inputFP);
	scanToNextVal(inputFP, NULL);
	getHeight(inputFP);
	scanToNextVal(inputFP, NULL);
    getColourRange(inputFP);
	fgetc(inputFP); // should only be one white space character between raster and colour range value
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
		if(strcmp(fileName, "") != 0) {
			remove(fileName); // if we have been writing to a file we delete it so it does not remain partially completed
		}
		exit(EXIT_FAILURE);
	}
}
