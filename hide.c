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

int main(int argc, char **argv) {
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t lineLength;

	/**
	fileType val is initialised to 0 before we reach the part of file that should be the fileType.
	If a valid filetype has been found, it will be set to 1 for plain PPM format (magic number P3) 
	or 2 for a raw PPM file (magic number P6)
	**/
	int fileType = 0; 

	// Constant values used for file type int, to make it easier to reason about in code
	int RAW_PPM = 2;
	int PLAIN_PPM = 1;
	
	int height = 0;
	int width = 0;
	int colourDepth = 0;

	int readingDimensions = 1;
	int readingColourDepth = 1;
	int readingData = 1;

	if(argc != 3) {
		printf("Incorrect number of arguments supplied, hide expects 2 arguments\n[1] path to ppm file to hide a message in\n[2] a name for the output ppm file\n");
	}
	// ./ppm-files/feep.ppm

	fp = fopen(argv[1], "r+");
	if (fp == NULL) {
		printf("Could not open supplied file: %s - Exiting\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (fgetc(fp) == 80) {
		if(fgetc(fp) == 54) {
			printf("File type is raw PPM (P6)\n");
			while(isspace(fgetc(fp){})); //skips through white space


		}
	} else {
		printf("Incorrect file format detected, aborting\n");
		exit(EXIT_FAILURE);
	}

	/**
	while (fgetc(fp) != "EOF") {
		printf("%c", fgetc(fp));
	}
	while ((lineLength = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", lineLength);
	    //printf("%s", line);
        int i;
		if(fileType == PLAIN_PPM && lineLength > 70) {
			printf("Detected a linelength of %lu, plain PPM files should not exceed 70 chars, exiting\n", lineLength);
			exit(EXIT_FAILURE);
		}
        for(i = 0; i < strlen(line); i++) {
            if(line[i] == 35) { // 35 is ascii code for # symbol
                printf("Found comment, skipping...\n");
                break;
            } else {
				if(!isspace(line[i])){
					if(!fileType) {
						if(line[i] == 80 && line[i+1] == 51) {
							fileType = PLAIN_PPM;
							printf("File type is plain PPM (P3)\n");
							break;
						} else if (line[i] == 80 && line[i+1] == 54) {
							fileType = RAW_PPM;
							printf("File type is raw PPM (P6)\n");
							break;
						} else {
							printf("Incorrect file format detected, aborting\n");
							exit(EXIT_FAILURE);
						}
					} else if (fileType == RAW_PPM) {
						if(readingDimensions) {
							int j = i;
							while(!isspace(line[j])){
								width = width * 10 + (line[j] - 48);
								j++;
							}
							while(isspace(line[j])) {
								j++;
							}
							while(!isspace(line[j]) && (j != lineLength - 1)) {
								height = height * 10 + (line[j] - 48);
								j++;
							}
							readingDimensions = 0;
							printf("Detected width = %d\n", width);
							printf("Detected height = %d\n", height);
							break;
						} else if (readingColourDepth) {
							int j = i;
							while(!isspace(line[j])){
								colourDepth = colourDepth * 10 + (line[j] - 48);
								j++;
							}
							printf("Detected colour depth = %d\n", colourDepth);

							if(colourDepth > 65535) {
								printf("Invalid colour depth found, must be less than 65536\n");
								exit(EXIT_FAILURE);
							} else if (colourDepth < 1) {
								printf("Invalid colour depth found, must be greater than 0\n");
								exit(EXIT_FAILURE);
							} else {
								readingColourDepth = 0;
							}
						} else if (readingData) {
							printf("%u\n", line[i]);
						}
					} else {
						printf("Unexpected behaviour enountered during filetype interpreting, aborting\n");
						exit(EXIT_FAILURE);
					}
				}				
			}
        }
	}
	
	free(line);
	fclose(fp);
	**/
	exit(EXIT_SUCCESS);
}