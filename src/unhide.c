 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 18/03/17
DESCRIPTION: unhide is a program that reads in a ppm image with a hidden message 
and returns the hidden message to standard output. 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include "unhideUtils.c"	// 

/* 
These are usually defined but just in case the test computers use some weird setup we 
define the success and failure variables for easier readability of the code and what 
constitutes a failed and a successful finish of execution
*/
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE	1
#endif

int argParse(int argc, char *argv[]) {
	if (argc >= 2) {
		if (strcmp("-m", argv[1]) == 0) {
			// expecting input of form "$ hide -m numberOfFiles inputBasename outputBasename"
			if(argc == 3) {
				return 'm'; // process file flag
			} 

		} else {
			// have to assume standard hide at this stage of form "$ hide inputPPM outputPPM"
			if(argc == 2) {
				return '0'; // default case flag
			}
		}

		return EXIT_FAILURE;

	} else {
		return EXIT_FAILURE;
	}
}

int main(int argc, char **argv) {
	int flagType;
	int count = 0;
	int numberOfFiles = 255; // start number of files at max and know it can only decrease when read
	char numberString[9];
	FILE *inputFP;
	
	
	flagType = argParse(argc, argv);

	switch(flagType) {
		case 'm':
			fprintf(stderr, "m flag detected\n");

			while (count < numberOfFiles) {
				snprintf(numberString, 9, "-%03d.ppm", count);
				char *output = malloc(strlen(argv[2]) + strlen(numberString) + 1);
				strcpy(output, argv[2]);
				strcat(output, numberString);
				inputFP = getUnhideFP(output, 1);
				numberOfFiles = getNumberOfFiles(inputFP);
				unhideMessage(inputFP, 1);
				fclose(inputFP);
				free(output);
				count += 1;
			}
			
			// handle multi file unhide here
			break;
		case '0':
			fprintf(stderr, "no flags detected\n");
			inputFP = getUnhideFP(argv[1], 0);
			unhideMessage(inputFP, 0);
			break;
	
		default:
			fprintf(stderr, "Incorrect arg format detected\nUnhide only supports 1 flag at a time or no flags\n");
			exit(EXIT_FAILURE);
	}

}


