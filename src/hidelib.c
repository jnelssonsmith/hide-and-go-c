#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "hidelib.h"

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


void hideMessage(int *byteIndexPtr, FILE *inputFP, FILE *outputFP){
	int bitToHide, currentNum, currentVal, compareBit;
	printf("Hiding message...\n");
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

	printf("Message hidden\n");
}