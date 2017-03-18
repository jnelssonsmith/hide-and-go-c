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
#define FINISHED_WRITING_SECRET 2

int isRawPPM(FILE *inputFile, FILE *outputFile);

int getWidth(FILE *inputFP);
int getHeight(FILE *inputFP);
int getColourRange(FILE *inputFP);
void scanToNextVal(FILE *inputFile, FILE *outputFile);

int getNextBitToHide(int *byteIndexPtr);
int canHideMessage(int width, int height);
void hideMessage(int *byteIndexPtr, FILE *inputFP, FILE *outputFP);

void writeColourRange(int colourRange, FILE *outputFP);
void writeWidth(int width, FILE *outputFP);
void writeHeight(int height, FILE *outputFP);

int main(int argc, char **argv) {
	
	FILE *inputFP;
	FILE *outputFP;
	

	int height;
	int width;
	int colourRange;

	int byteIndex = 7;
	int *byteIndexPtr = &byteIndex;
	int index = 0;
	int *indexPtr = &index;

	printf("argc: %d\n", argc);
	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
		exit(EXIT_FAILURE);
	} else {
		inputFP = fopen(argv[1], "rb");
		if (inputFP == NULL) {
			printf("Could not open supplied file: %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		outputFP = fopen(argv[2], "wb");

		if(isRawPPM(inputFP, outputFP)){
			scanToNextVal(inputFP, outputFP);
			
			width = getWidth(inputFP);
			writeWidth(width, outputFP);

			scanToNextVal(inputFP, outputFP);
			
			height = getHeight(inputFP);
			writeHeight(height, outputFP);

			scanToNextVal(inputFP, outputFP);
			
			colourRange = getColourRange(inputFP);
			writeColourRange(colourRange, outputFP);
			
			scanToNextVal(inputFP, outputFP);

			if(canHideMessage(width, height)){
				hideMessage(byteIndexPtr, inputFP, outputFP);
			} else {
				printf("Message is too large for given file, please use a smaller message or a larger file\n");
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Incorrect file format detected, aborting\n");
			exit(EXIT_FAILURE);
		}

		fclose(inputFP);
		fclose(outputFP);
		exit(EXIT_SUCCESS);
	}
}


int isRawPPM(FILE *inputFile, FILE *outputFile) {
	if (fgetc(inputFile) == 'P') {
		if(fgetc(inputFile) == '6') {
			printf("File type is raw PPM (P6)\n");
			fputc('P', outputFile);
			fputc('6', outputFile);
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
			printf("Width: %d\n", width);
			return width;
		} else {
			width = width * 10 + (currentChar - 48);
		}
	}
}


void writeWidth(int width, FILE *outputFP) {
	fprintf(outputFP, "%d", width);
}

int getHeight(FILE *inputFP) {
	int currentChar;
	int height = 0; 
	while(1) {
		currentChar = fgetc(inputFP);
		if(isspace(currentChar)){
			ungetc(currentChar, inputFP);
			printf("Height: %d\n", height);
			return height;
		} else {
			height = height * 10 + (currentChar - 48);
		}
	}
}

void writeHeight(int height, FILE *outputFP) {
	fprintf(outputFP, "%d", height);
}

int getColourRange(FILE *inputFP) {
	int currentChar;
	int colourRange = 0; 
	while(1) {
		
		currentChar = fgetc(inputFP);
		if(isspace(currentChar)){
			if(currentChar < 1) {
				printf("Colour range value must be greater than 0, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else if (currentChar > 65535) {
				printf("Colour range value must be less than 65546, detected: %d", colourRange);
				exit(EXIT_FAILURE);
			} else {
				printf("Colour range: 0 to %d\n", colourRange);
				ungetc(currentChar, inputFP); //give back last grabbed value
				return colourRange;
			}
		} else {
			colourRange = colourRange * 10 + (currentChar - 48);
		}
	}
}

void writeColourRange(int colourRange, FILE *outputFP) {
	fprintf(outputFP, "%d", colourRange);
}

void scanToNextVal(FILE *inputFile, FILE *outputFile) {
	int temp;
	while(1){
		temp = fgetc(inputFile);
		if(isspace(temp)) {
			fputc(temp, outputFile);
		} else {
			break;
		}
	} 
	ungetc(temp, inputFile);
}


int canHideMessage(int width, int height) {
	int values = width * height * 3;  // the number of bytes we have available to hide a message index
	//int totalBits = messageLength * 8; // the number of bits we need to hide
	int totalBits = 100; //hardCodeforNow
	if(totalBits > values) {
		return 0;
	} else {
		printf("Detected %d bytes to hide within\n", values);
		printf("Detected %d bits to hide\n", totalBits);
		return 1;
	}

} 

int getNextBitToHide(int *byteIndexPtr) {
	int currentMessageChar;
	int bitVal;

	currentMessageChar = fgetc(stdin);
	
	if(currentMessageChar != EOF) {
		printf("%c", currentMessageChar);
		if(*byteIndexPtr == 7) {
			//printf("\n%c  = ", message[*indexPtr]);
		}

		bitVal = currentMessageChar >> *byteIndexPtr & 1;
		//printf("%d", bitVal);

		*byteIndexPtr -= 1;
		if(*byteIndexPtr == -1) {
			*byteIndexPtr = 7;
		}

		return bitVal;

	} else {
		if(*byteIndexPtr == -1) {
			//printf("\n");
			return FINISHED_WRITING_SECRET;
		} else {
			if(*byteIndexPtr == 7) {
				//printf("\n\\0 = ");
			}
			//printf("%d", 0);
			*byteIndexPtr -= 1;
			return 0;
		}
	}
}


 
void hideMessage(int *byteIndexPtr, FILE *inputFP, FILE *outputFP){
	int bitToHide, currentNum, currentVal, compareBit;
	while(1) {
		bitToHide = getNextBitToHide(byteIndexPtr);
		if(bitToHide == FINISHED_WRITING_SECRET) {
			break;
		} else {
			currentNum = fgetc(inputFP);
			compareBit = currentNum % 2;
			if(compareBit && !bitToHide) {
				fputc(currentNum - 1, outputFP);
			} else if (!compareBit && bitToHide) {
				fputc(currentNum + 1, outputFP);
			} else {
				fputc(currentNum, outputFP);
			}
		}
	}

	while (inputFP) {
		currentVal = fgetc(inputFP);
		if(currentVal == EOF) {
			break;
		} else {
			fputc(currentVal, outputFP);
		}
	}
}
}
