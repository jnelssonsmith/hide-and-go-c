 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 26/03/17
DESCRIPTION: Here we implement the steganography library functions defined in steglib.h. 
Should you want more detailed information about the inputs, outputs and reasons for these 
functions please look at the header file. 
*/

#include <stdio.h>
#include <stdlib.h>
#include "steglib.h"

int getSupportedImageBytes(int width, int height) {
	int availableBytes = width * height * 3;  // the number of bytes we have available to hide a message in 
	fprintf(stderr, "Found %d bytes to hide a message in\n", availableBytes);
	return availableBytes; 
}

int canHideMessage(int messageSize, int maxSizeSupportedByImage) {
	/* because the message size is counted byte by byte, but since we hide a message bit by bit, we multiply the bytes by 8
	   note that we also add 4 bytes to the message size because we hide the message size itself as a 32 bit int */
	int numberOfBitsToHide = (messageSize + 4) * 8;  

	/* because we hide the size of a message inside the file we must set a certain max int so we know when to stop reading 
	   the message. I have chosen a 32 bit integer to represent the message size, this carries with it the restriction 
	   that a message cannot by over ~2GB */
	if(messageSize > 2147483647) {
		fprintf(stderr, "Message is too large, max size supported is 2GB\n");
		return MESSAGE_WRITE_ERROR;
	} else if (numberOfBitsToHide > maxSizeSupportedByImage) {
		fprintf(stderr, "Message is too large for given file...\nNeed to hide %d bits, but only have %d bytes to hide within\n", numberOfBitsToHide, maxSizeSupportedByImage);
		return MESSAGE_WRITE_ERROR;
	} else {
		return MESSAGE_WRITE_SUCCESS;
	}
}

void hideMessageSize(int messageSize, FILE *inputFP, FILE *outputFP) {
	int mask, 				// the bit mask we construct for breaking the 32 bit int into it's 1 and 0 values
		bitToHide, 			// represents the bit we want to hide at any given time
		currentNum,         // the current r,g or b value that we have read from the input ppm file 
		compareBit;         // the last bit of the current number we are working on (the bit we have to compare with the bit to hide)
	
	fprintf(stderr, "Hiding size %d as 32 bits: ", messageSize);
	for(int i=31; i > -1; i--) {
		mask = 1 << i; // constuct a bit mask to get our target bit
		bitToHide = (messageSize & mask) >> i;  // apply the mask and bit shift it so we are left with only one bit

		currentNum = fgetc(inputFP);
		compareBit = currentNum % 2;  // the last bit of a number will always be the remainder of dividing by 2, 0 if even, 1 if odd


		/* if the bits differ we correct so that the last bit matches the bit to hide, otherwise we write out as normal because 
		   they are the same */
		if(compareBit && !bitToHide) {
			fputc(currentNum - 1, outputFP);
		} else if (!compareBit && bitToHide) {
			fputc(currentNum + 1, outputFP);
		} else {
			fputc(currentNum, outputFP);
		}
		
		fprintf(stderr, "%d", bitToHide);
	}

	fprintf(stderr, "\n");
}


int hideMessage(int maxSizeSupportedByImage, FILE *inputFP, FILE *outputFP, int multiMode){
	int bitToHide, 			// the current bit we wish to hide in the image
		currentNum, 		// the current r,g or b pixel value that we are working on 
		compareBit, 		// the last bit of the read in r,g or b value 
		currentChar,		// the current character we have read from the message to hide from standard input
		messageSize = 0,	// the size of the message to hide
		error,				// a int used for error signalling
		count;
	long positionInFile;	// the position in the file we can use to seek to later when we write in the size of the message


	// we will need to revist the start of the pixel raster later to write in the message size so we save it here
	positionInFile = ftell(inputFP);
	
	// we do not know the message size yet so we write through 32 bytes worth of space for the 32 bit int to be encoded later
	for(int i=0; i < 32; i++){
		currentNum = fgetc(inputFP);
		fputc(currentNum, outputFP);
	}

	fprintf(stderr, "Hiding message...\n");
	currentChar = fgetc(stdin);
	count = 0;

	while(currentChar != EOF && maxSizeSupportedByImage > ((messageSize * 8) + 264)) {
		/* while the current character is not the end of the file we iterate through to get each of the 
		   8 bytes of the char, for each of these bits we compare them to the r,g,b value and make the 
		   necessary changes so that the bit matches when we write out */
	
		for(int j=7; j > -1; j--) {
			bitToHide = currentChar >> j & 1;
			currentNum = fgetc(inputFP);
			count += 1;
			compareBit = currentNum % 2;

			if(compareBit && !bitToHide) {
				fputc(currentNum - 1, outputFP);
			} else if (!compareBit && bitToHide) {
				fputc(currentNum + 1, outputFP);
			} else {
				fputc(currentNum, outputFP);
			}
		}

		messageSize += 1;

		if(maxSizeSupportedByImage > ((messageSize * 8) + 264)) {
			currentChar = fgetc(stdin);
			// we only read from standard in if we have not gone over
		} else {
			fprintf(stderr, "Hit the end of the size at messageSize = %d, or %d exactly\n", messageSize, ((messageSize * 8) + 256));
		}
		
		
		

		/* we check whether the message fits seeing as we don't know the message size before hand, if we reach a point 
		   where it doesn't fit, we exit the loop return an error */
		if(!multiMode) {
			error = canHideMessage(messageSize, maxSizeSupportedByImage);
			if(error) {
				return MESSAGE_WRITE_ERROR;
			}
		}
	}
	
	fprintf(stderr, "Message size is: %d or %d bits\n", messageSize, messageSize * 8);
	
	// now that the message is hidden, we write out the remaining bytes in the file as they appear originally 
	while (inputFP) {
		//fprintf(stderr, "Still entered loop even though we ran out of space\n");
		currentNum = fgetc(inputFP);
		if(currentNum == EOF) {
			break;
		} else {
			fputc(currentNum, outputFP);
		}
	}	

	/* to save time we have written the message straight to the output file without first checking size, now that 
	we know it fits and we have the size, we can seek back to the start and write in the message size as 32 bits */
	fseek(inputFP, positionInFile, SEEK_SET);
	fseek(outputFP, positionInFile, SEEK_SET);
	hideMessageSize(messageSize, inputFP, outputFP);

	fprintf(stderr, "Message hidden\n");
	return MESSAGE_WRITE_SUCCESS;

}

int readSizeOfSecretMessage(FILE *inputFP){
	int currentNum, // the current r,g or b val we have read from the image		
	    lastBit,    // the last bit of the current num 
		size = 0;   // the size of the secret message, as we read we increment this until we reach the size. 

	fprintf(stderr, "Message size found: ");
	// we read through 32 bytes and construct the size of the secret message back out
	for(int i=31; i > -1; i--) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		fprintf(stderr, "%d", lastBit);
		size += lastBit << i;
	}

	// once we have read we return the size we found
	fprintf(stderr, " or %d in decimal\n", size);
	return size;
}

void readSecretMessage(int size, FILE *inputFP) {	
	int currentNum, 				// the current number we have read from the pixel raster (r,g, or b value)
		lastBit, 					// the last bit of the current number that contains a bit of the secret message
		currentChar = 0, 			// we construct the chars out, so we set the char value to 0 initially and get the full number out over 8 bits
		bitPosition = 7,			// we start by working from the largest value and decrementing 8 times, this helps us with our bit shifting
		numberOfBits = size * 8;	// we keep reading until the number of bits is reached
	
	for(int i=0; i < numberOfBits; i++) {
		currentNum = fgetc(inputFP);
		lastBit = currentNum % 2;
		currentChar += lastBit << bitPosition; // same as saying 1 * 2^7 + 0 * 2^6 + ... etc until we reach 8th bit, it's converting binary to decimal
		bitPosition -= 1;

		if(bitPosition < 0) {
			// once we reach the 8th bit we reset for the next 8 bits
			fputc(currentChar, stdout);
			bitPosition = 7;
			currentChar = 0;
		}
	}
}