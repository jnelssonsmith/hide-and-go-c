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
#include <math.h>
#include "ppmlib.h"
#include "steglib.h"

int main(int argc, char **argv) {
	
	FILE *inputFP;	
	
	if(argc != 2) {
		printf("Incorrect number of arguments supplied, unhide expects 1 argument\n[1] path to a ppm file with a hidden message in it\n");
		exit(EXIT_FAILURE);
	} else {
		inputFP = fopen(argv[1], "rb");
		if (inputFP == NULL) {
			printf("Could not open supplied file: %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}

		if(isRawPPM(inputFP, NULL)){
			scanToImageData(inputFP);
            // size = readSizeOfSecretMessage(inputFP);
            // readSecretMessage(size, inputFP);
		} else {
			printf("Incorrect file format detected, aborting\n");
			exit(EXIT_FAILURE);
		}

		fclose(inputFP);
		exit(EXIT_SUCCESS);
	}
}



