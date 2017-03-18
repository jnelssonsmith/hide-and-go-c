#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "steglib.h"

int canHideMessage(int width, int height, int messageSize) {
	int availableBytes = width * height * 3;  // the number of bytes we have available to hide a message index
	int necessaryBytes = messageSize + 2; // includes the 2 bytes we use for the integer 
	int necessaryBits = necessaryBytes * 8;
	if(necessaryBits > availableBytes) {
		return 0;
	} else {
		printf("Detected %d bytes to hide within\n", availableBytes);
		printf("Detected %d bits to hide (16 bits for message size and %d bits for message itself)\n", necessaryBits, messageSize * 8);
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
		return FINISHED_WRITING_SECRET;
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
	printf("Message size: %d bytes (%d bits)\n", size, size * 8);
	return size;
}

void hideMessageSize(int messageSize, FILE *inputFP, FILE *outputFP) {
	int mask, bitToHide, maskedMessageSize, currentNum, compareBit;
	printf("Hiding size %d as 16 bits: ", messageSize);
	for(int i=15; i > -1; i--) {
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


void hideMessage(int messageSize, char message[65536], FILE *inputFP, FILE *outputFP){
	int bitToHide, 
		currentNum, 
		currentVal, 
		compareBit, 
		currentChar;

	printf("Hiding message...\n");
	for(int i=0; i < messageSize; i++){
		currentChar = message[i];
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
	}

	while (inputFP) {
		currentVal = fgetc(inputFP);
		if(currentVal == EOF) {
			break;
		} else {
			fputc(currentVal, outputFP);
		}
	}

	printf("Message hidden\n");
}

int readSizeOfSecretMessage(FILE *inputFP){
	int currentNum, lastBit, size = 0;
	printf("Message size found: ");
	for(int i=15; i > -1; i--) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		printf("%d", lastBit);
		size += lastBit << i;
	}

	printf(" or %d in decimal\n", size);
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