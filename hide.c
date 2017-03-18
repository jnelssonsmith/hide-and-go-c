 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 18/03/17
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
int getMessageSize(FILE *messageFP, char message[]);

void scanToNextVal(FILE *inputFile, FILE *outputFile);

int getNextBitToHide(int *byteIndexPtr);
int canHideMessage(int width, int height, int messageSize);
void hideMessage(int *byteIndexPtr, FILE *inputFP, FILE *outputFP);

void writeColourRange(int colourRange, FILE *outputFP);
void writeWidth(int width, FILE *outputFP);
void writeHeight(int height, FILE *outputFP);

int main(int argc, char **argv) {
	
	FILE *inputFP;
	FILE *outputFP;
	
	char message[65536];
	int messageSize;
	int height;
	int width;
	int colourRange;
	int scanCount;

	int byteIndex = 7;
	int *byteIndexPtr = &byteIndex;

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

		messageSize = getMessageSize(stdin, message);
		if(messageSize == -1) {
			printf("Message too large, hide only supports messages smaller than 65536 bytes\n");
			exit(EXIT_FAILURE);
		}

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

			if(canHideMessage(width, height, messageSize)){
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
			if(outputFile != NULL) {
				fputc(temp, outputFile);
			}
		} else {
			break;
		}
	} 
	ungetc(temp, inputFile);
}


int canHideMessage(int width, int height, int messageSize) {
	int availableBytes = width * height * 3;  // the number of bytes we have available to hide a message index
	int necessaryBytes = messageSize + 2; // includes the 2 bytes we use for the integer 
	int necessaryBits = necessaryBytes * 8;
	if(necessaryBits > availableBytes) {
		return 0;
	} else {
		printf("Detected %d bytes to hide within\n", availableBytes);
		printf("Detected %d bits to hide\n", necessaryBits);
		return 1;
	}

} 

int getNextBitToHide(int *byteIndexPtr) {
	int currentMessageChar;
	int bitVal;

	currentMessageChar = fgetc(stdin);
	
	if(currentMessageChar != EOF) {
		printf("%c", currentMessageChar);

		bitVal = currentMessageChar >> *byteIndexPtr & 1;

		*byteIndexPtr -= 1;
		if(*byteIndexPtr == -1) {
			*byteIndexPtr = 7;
		}

		return bitVal;

	} else {
		if(*byteIndexPtr == -1) {
			return FINISHED_WRITING_SECRET;
		} else {
			*byteIndexPtr -= 1;
			return 0;
		}
	}
}

int getMessageSize(FILE *messageFP, char message[65536]){
	int size = 0;
	int currentChar;
	while(1) {
		if(size >= 65537) {
			return -1;
		} else {
			currentChar = fgetc(messageFP);
			if(currentChar == EOF) {
				break;
			} else {
				message[size] = currentChar;
				size += 1;
			}
		}

	}
	printf("Message size: %d\n", size);
	return size;
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
