/*
NAME: Joshua Nelsson-Smith
START DATE: 21/04/17
LAST MODIFIED: 06/05/17
DESCRIPTION: The hide utils are a group of functions that allow for the handling of 
the different paths that are available to the program via flags
*/
#ifndef HIDEUTILS_H_
#define HIDEUTILS_H_

/*
standard hide message is the base function for dealing with the hiding of a message, 
it takes in the input PPM file name, the output PPM file name, whether or not it is in multi mode 
(signified by the -m flag) and if the -m flag is being used, the number of files variable will be 
between 0 and 255
*/
void standardHideMessage(char *inputPPM, char *outputPPM, int multiMode, int numberOfFiles);

/*
parallel execute takes in an input file name that follows the format definition defined for the -p
flag, that is, a text file full of newline seperated strings adhearing to:
messageFile inputPPMName.ppm outputPPMname.ppm 
*/
void parallelExecute(char *inputFile);

/* 
The draw PPM Image function is incharge of interacting with the SDL library and calculating 
drawing the pixels for the two images to the screen. To do this it requires the width and height of 
one of the images (knowing that both should be the same dimensions), and input file pointer to read from 
and an output file pointer to also read from. (as we want to open them both)
*/
void drawPPMImage(int width, int height, FILE *inputFP, FILE *outputFP);

/*
The display image function is in charge of dealing with the logic behind the -s flag, wherein we display an 
image side by side with the same image with a message hidden in it. It takes in an input and output ppm file string 
as arguments.
*/
void displayImage(char *inputFile, char *outputFile);

/* 
The multi hide message function is in charge of dealing with the -m flag, wherein we hide a message across 
multiple files following the naming convention inputBaseName-00x. The function takes the number of files as 
input and the inputBaseName and the outputBaseName to use for the calculation of the various file names
*/
void multiHideMessage(int numberOfFiles, char *inputBaseName, char *outputBaseName);
#endif