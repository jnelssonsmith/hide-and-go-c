#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "steglib.h"

int getSupportedImageBytes(int width, int height) {
	int availableBytes = width * height * 3;  // the number of bytes we have available to hide a message index
	
	/*
	int necessaryBytes = messageSize + 2; // includes the 2 bytes we use for the integer 
	int necessaryBits = necessaryBytes * 8;
	if(necessaryBits > availableBytes) {
		return 0;
	} else {
		fprintf(stderr, "Detected %d bytes to hide within\n", availableBytes);
		fprintf(stderr, "Detected %d bits to hide (16 bits for message size and %d bits for message itself)\n", necessaryBits, messageSize * 8);
		return 1;
	}
	*/

	return availableBytes;

} 

/*
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
	fprintf(stderr, "Message size: %d bytes (%d bits)\n", size, size * 8);
	return size;
}
*/

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


void hideMessage(int maxSizeSupportedByImage, FILE *inputFP, FILE *outputFP){
	int bitToHide, 
		currentNum, 
		currentVal, 
		compareBit, 
		currentChar,
		messageSize = 0;
	long positionInFile;

	positionInFile = ftell(inputFP);
	fprintf(stderr, "pos before: %lu\n", positionInFile);
	
	
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
		if(messageSize > 2147483647) {
			fprintf(stderr, "Message is too large, max size supported is 2GB\n");
			exit(EXIT_FAILURE);
		} else if (messageSize > maxSizeSupportedByImage) {
			fprintf(stderr, "Message is too large for given file\n");
			exit(EXIT_FAILURE);
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

	fseek(inputFP, positionInFile, SEEK_SET);
	fseek(outputFP, positionInFile, SEEK_SET);
	positionInFile = ftell(inputFP);
	fprintf(stderr, "pos after: %lu\n", positionInFile);
	hideMessageSize(messageSize, inputFP, outputFP);

	fprintf(stderr, "Message hidden\n");
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