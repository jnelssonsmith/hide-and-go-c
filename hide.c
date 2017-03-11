 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 10/03/17
DESCRIPTION: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	/**
	fileType val is initialised to 0 before we reach the part of file that should be the fileType.
	If a valid filetype has been found, it will be set to 1 for plain PPM format (magic number P3) 
	or 2 for a raw PPM file (magic number P6)
	**/
	int fileType = 0; 
 
	fp = fopen("./ppm-files/feep.ppm", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	
	while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
	    //printf("%s", line);
        int i;
        for(i = 0; i < strlen(line); i++) {
            if(line[i] == 35) { // 35 is ascii code for # symbol
                printf("Found comment, skipping...\n");
                break;
            } else {
				if(!isspace(line[i])){
					if(!fileType) {
						if(line[i] == 80 && line[i+1] == 51) {
							fileType = 1;
							printf("File type is plain PPM (P3)\n");
							break;
						} else if (line[i] == 80 && line[i+1] == 54) {
							fileType = 2;
							printf("File type is raw PPM (P6)\n");
							break;
						} else {
							printf("Incorrect file format detected, aborting");
							exit(EXIT_FAILURE);
						}
					}
				}				
			}
        }
	}
 
	free(line);
	fclose(fp);
	exit(EXIT_SUCCESS);
}