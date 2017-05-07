/*
NAME: Joshua Nelsson-Smith
START DATE: 21/04/17
LAST MODIFIED: 06/05/17
DESCRIPTION: The unhide utils are a group of functions that allow for the handling of 
the different paths that are available to the program via flags
*/
#ifndef UNHIDEUTILS_H_
#define UNHIDEUTILS_H_

/*
unhide message is the base function for unhiding a message, it takes in a ready and processed file pointer already positioned at the 
pixel raster
*/
void unhideMessage(FILE *inputFP);

/*
the get unhide FP takes in an input file name with a message hidden in it, it scans through, ensures the file 
is valid and has all the relevant PPM headers and then returns a file pointer pointed at the start of the pixel raster
*/
FILE* getUnhideFP(char *inputName); 

/*
Get number of files is used when the -m flag is used, it reads the first 8 bytes of the given file pointer
to retrieve the 8 bit number representing how many files were are expecting and returns it
*/
int getNumberOfFiles(FILE *inputFP);

#endif