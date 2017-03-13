 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 12/03/17
DESCRIPTION: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isRawPPM(FILE *fp);
int getWidth(FILE *fp);
int getHeight(FILE *fp);
void scanToNextVal(FILE *fp);
int getColourRange(FILE *fp);

int main(int argc, char **argv) {
	FILE *fp;

	int height;
	int width;
	int colourRange;

	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
	}

	fp = fopen(argv[1], "r+");
	if (fp == NULL) {
		printf("Could not open supplied file: %s - Exiting\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if(isRawPPM(fp)){
		scanToNextVal(fp);
		width = getWidth(fp);
		scanToNextVal(fp);
		height = getHeight(fp);
		scanToNextVal(fp);
		colourRange = getColourRange(fp);
	} else {
		printf("Incorrect file format detected, aborting\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}


int isRawPPM(FILE *fp) {
	if (fgetc(fp) == 80) {
		if(fgetc(fp) == 54) {
			printf("File type is raw PPM (P6)\n");
			return 1;
		}
	}

	return 0;
}

int getWidth(FILE *fp) {
	int currentChar;
	int width = 0; 
	while(1) {
		
		currentChar = fgetc(fp);
		if(isspace(currentChar)){
			printf("Width: %d\n", width);
			return width;
		} else {
			width = width * 10 + (currentChar - 48);
		}
	}
}

int getHeight(FILE *fp) {
	int currentChar;
	int height = 0; 
	while(1) {
		
		currentChar = fgetc(fp);
		if(isspace(currentChar)){
			printf("Height: %d\n", height);
			return height;
		} else {
			height = height * 10 + (currentChar - 48);
		}
	}
}

int getColourRange(FILE *fp) {
	int currentChar;
	int colourRange = 0; 
	while(1) {
		
		currentChar = fgetc(fp);
		if(isspace(currentChar)){
			if(currentChar < 1) {
				printf("Colour range value must be greater than 0, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else if (currentChar > 65535) {
				printf("Colour range value must be less than 65546, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else {
				printf("Colour range: 0 to %d\n", colourRange);
				return colourRange;
			}
		} else {
			colourRange = colourRange * 10 + (currentChar - 48);
		}
	}
}

void scanToNextVal(FILE *fp) {
	int temp;
	while(isspace(temp = fgetc(fp))){} //do nothing
	ungetc(temp, fp);
}


