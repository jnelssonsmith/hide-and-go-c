#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "steglib.h"

int getSupportedImageBytes(int width, int height) {
	int availableBytes = width * height * 3;  // the number of bytes we have available to hide a message index
	fprintf(stderr, "Found %d bytes to hide a message in\n", availableBytes);
	return availableBytes;

}

int canHideMessage(int messageSize, int maxSizeSupportedByImage) {
	int numberOfBitsToHide = messageSize * 8; 

	if(messageSize > 2147483647) {
		fprintf(stderr, "Message is too large, max size supported is 2GB\n");
		return 0;
	} else if (numberOfBitsToHide > maxSizeSupportedByImage) {
		fprintf(stderr, "Message is too large for given file...\nNeed to hide %d bits, but only have %d bytes to hide within\n", numberOfBitsToHide, maxSizeSupportedByImage);
		return 0;
	} else {
		return 1;
	}
}

void hideMessageSize(int messageSize, FILE *inputFP, FILE *outputFP) {
	int mask, bitToHide, maskedMessageSize, currentNum, compareBit;
	fprintf(stderr, "Hiding size %d as 32 bits: ", messageSize);
	for(int i=31; i > -1; i--) {
		mask = 1 << i;
		maskedMessageSize = messageSize & mask;
		bitToHide = maskedMessageSize >> i;

		currentNum = fgetc(inputFP);
		compareBit = currentNum % 2;

		if(compareBit && !bitToHide) {
			fputc(currentNum - 1, outputFP);
		} else if (!compareBit && bitToHide) {
			fputc(currentNum + 1, outputFP);
		} else {
			fputc(currentNum, outputFP);
		}

		
		printf("%d", bitToHide);
	}

	printf("\n");
}


int hideMessage(int maxSizeSupportedByImage, FILE *inputFP, FILE *outputFP){
	int bitToHide, 
		currentNum, 
		currentVal, 
		compareBit, 
		currentChar,
		messageSize = 0;
	long positionInFile;

	positionInFile = ftell(inputFP);
	
	for(int i=0; i < 32; i++){
		currentVal = fgetc(inputFP);
		fputc(currentVal, outputFP);
	}

	fprintf(stderr, "Hiding message...\n");
	currentChar = fgetc(stdin);
	while(currentChar != EOF) {
		for(int j=7; j > -1; j--) {
			bitToHide = currentChar >> j & 1;
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

		currentChar = fgetc(stdin);
		messageSize += 1;
		
	}
	
	fprintf(stderr, "Message size is: %d or %d bits\n", messageSize, messageSize * 8);
	
	while (inputFP) {
		currentVal = fgetc(inputFP);
		if(currentVal == EOF) {
			break;
		} else {
			fputc(currentVal, outputFP);
		}
	}	

	if(canHideMessage(messageSize, maxSizeSupportedByImage)){
		fseek(inputFP, positionInFile, SEEK_SET);
		fseek(outputFP, positionInFile, SEEK_SET);
		positionInFile = ftell(inputFP);
		hideMessageSize(messageSize, inputFP, outputFP);
		fprintf(stderr, "Message hidden\n");
		return 1;
	} else {
		return 0;
	}
}

int readSizeOfSecretMessage(FILE *inputFP){
	int currentNum, lastBit, size = 0;
	fprintf(stderr, "Message size found: ");
	for(int i=31; i > -1; i--) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		fprintf(stderr, "%d", lastBit);
		size += lastBit << i;
	}

	fprintf(stderr, " or %d in decimal\n", size);
	return size;
}

void readSecretMessage(int size, FILE *inputFP) {	
	int currentNum, 
		lastBit, 
		currentChar = 0, 
		bitPosition = 7,
		numberOfBits = size * 8;
	
	for(int i=0; i < numberOfBits; i++) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		currentChar += lastBit << bitPosition;
		bitPosition -= 1;

		if(bitPosition < 0) {
			fputc(currentChar, stdout);
			bitPosition = 7;
			currentChar = 0;
		}
	}
}