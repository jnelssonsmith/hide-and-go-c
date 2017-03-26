 /*
NAME: Joshua Nelsson-Smith
START DATE: 19/03/17
LAST MODIFIED: 24/03/17
DESCRIPTION: The ppm library defines a group of functions that are useful
when dealing with the PPM image format, specifically raw PPM's (P6), but the library
could easily be extended to include functions to handle P3 format PPMs. 
*/
#ifndef PPMLIB_H_
#define PPMLIB_H_

/*
isRawPPM calculates whether a given file is a valid PPM format (P6). 
If it is successful then it returns PPM_READ_SUCCESS otherwise PPM_READ_ERROR
The inputs are the input file to read from, and the output file to write to. 
Should you not want to write to an output file, you can simply pass in NULL as the 
output file. 
*/
int isRawPPM(FILE *inputFile, FILE *outputFile);

/*
getWidth returns the width value given by a PPM file.
It has the precondition that the given file pointer is at the right place to begin 
reading in the width. 
The input for this function is the input file from which you want to read the width.
*/
int getWidth(FILE *inputFP);

/*
getHeight returns the height value given by a PPM file.
It has the precondition that the given file pointer is at the right place to begin 
reading in the height. 
The input for this function is the input file from which you want to read the height.
*/
int getHeight(FILE *inputFP);

/*
getColourRange returns the colour range value given by a PPM file.
If the colour range is not in the valid range (1-255) then it will return an error.
It has the precondition that the given file pointer is at the right place to begin 
reading in the colour range. 
The input for this function is the input file from which you want to read the colour range.
*/
int getColourRange(FILE *inputFP);

/*
scanToNextVal scans past whitespace and comments, it is useful to get the pointer to the start of
the next useful bit of info in the PPM file (the format, the width, height, colour range or actual colour raster)
The function takes in an input file to read from and an output file pointer to write to, if the output file pointer
is NULL then the function will not write out to it. 
*/
void scanToNextVal(FILE *inputFile, FILE *outputFile);

/*
scanToImageData is a useful function to use if you need to just scan to the section of a ppm that contains 
the colour raster. It makes use of all of the functions above and leaves the file pointer at the start of the 
raster.
*/
void scanToImageData(FILE *inputFP);

/*
To ensure the program exits gracefully (doesn't leave half written to files or leave things open) 
we define a custom exit gracefully function. It is useful because if for some reason the program fails 
during some step of execution with the output file being created, we will be left with a half written to 
file. This function attempts to cover up any of our evil deeds should they fail, otherwise it does the busy work
of ensuring files are closed and the correct exit signal is broadcast. 
*/
void exitGracefully(int error, char fileName[], FILE *inputFP, FILE *outputFP);

#endif

// These three definitions make the code more readable and can be used for signalling to the calling functions 
#ifndef PPM_READ_SUCCESS
#define PPM_READ_SUCCESS	     0
#endif

#ifndef PPM_READ_ERROR
#define PPM_READ_ERROR		     1
#endif

// need to use -1 for this because can't use the usual 1 and 0 for error checking
#ifndef PPM_COLOUR_READ_ERROR
#define PPM_COLOUR_READ_ERROR   -1
#endif