 /*
NAME: Joshua Nelsson-Smith
START DATE: 10/03/17
LAST MODIFIED: 25/03/17
DESCRIPTION: hide is a program that reads in a ppm image as it's first argument 
and an output file name as it's second argument. It then takes in a message to 
hide within the ppm image through stdin. If there are no errors with the 
program, hide will hide the stdin message inside the given ppm image and 
output it as a file with the name given by the second argument.
*/

#include <stdio.h>
#include <stdlib.h> 
#include <argp.h>
#include <unistd.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages

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

/*
choices are

hide inputPPM outputPPM
hide -m numberOfFiles inputBasename outputBasename
hide -p instructionFile
hide -s inputPPM outputPPM
*/

const char *argp_program_version = "hide 1.0";

const char *argp_program_bug_address = "https://github.com/Joshua-Xavier/hide-and-go-c/issues/new";

struct arguments {
  char *args[2];            		/* inputPPM and outputPPM */
  int numberOfFiles;      		  /* Argument for -m */
  char *instructionFile;    		/* Argument for -p */
  int sideBySide; 							/* Indicates precence of -s flag */
};

static struct argp_option options[] =
{
  {"multi", 'm', "numberOfFiles", 0, "Hide message across numberOfFiles files"},
  {"process", 'p', "instructionFile", 0, "use instructionFile as an instruction file for processing many ppm messages"},
  {"sideBySide", 's', 0, 0, "Show the input and output ppm files side by side"},
  {0}
};

int flagType = '0';

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;

  switch (key) {
    case 'm':
			if(flagType != '0') {
				flagType = -1;
			} else {
				flagType = 'm';
			} 

			arguments->numberOfFiles = atoi(arg);
      break;

    case 'p':
			if(flagType != '0') {
				flagType = -1;
			} else {
				flagType = 'p';
			}
      arguments->instructionFile = arg;
			return ARGP_KEY_SUCCESS; 						// the p flag only takes one arg and doesnt use input and output so we leave early
			
    case 's':
			if(flagType != '0') {
				flagType = -1;
			} else {
				flagType = 's';
			}
      arguments->sideBySide = 1;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2) {
	  		argp_usage(state);
	  	}
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2){
	  		argp_usage (state);
			}
      break;
			
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static char args_doc[] = "inputPPM ouputPPM";
static char doc[] = "hide -- Simple explanation here..";
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
	
	FILE *inputFP,	// the file pointer to the input ppm image
		 	 *outputFP; // the file pointer to the output ppm image with the hidden message inside
	
	struct arguments arguments;
	
	int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				      			// an error storing variable, 1 on error, 0 on no error
	
	arguments.sideBySide = 0;
	arguments.numberOfFiles = 0;
	arguments.instructionFile = "(notSupplied)";
	arguments.args[0] = "(notSupplied)";
	arguments.args[1] = "(notSupplied)";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	switch(flagType) {
		case 's':
			printf("s flag detected\n");
			break;
		case 'p':
			printf("p flag detected\n");
			break;
		case 'm':
			printf("m flag detected\n");
			break;
		case '0':
			printf("no flags detected\n");
			break;
		default:
			printf("Too many flags detected\nHide only supports 1 flag at a time or no flags\n");
			exit(EXIT_FAILURE);
	}
	

	
	inputFP = fopen(arguments.args[0], "rb");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	
	outputFP = fopen(argv[2], "wb");
	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, outputFP);
	if(error) {
		printf("Incorrect file format detected, aborting\n");
		exitGracefully(error, argv[2], inputFP, outputFP);
	}
	
	// scan through width, heigh and colour range values
	scanToNextVal(inputFP, outputFP);
	
	width = getWidth(inputFP);
	fprintf(outputFP, "%d", width);
	
	scanToNextVal(inputFP, outputFP);
	
	height = getHeight(inputFP);
	fprintf(outputFP, "%d", height);

	scanToNextVal(inputFP, outputFP);
	
	colourRange = getColourRange(inputFP);
	if(colourRange == PPM_COLOUR_READ_ERROR) {
		exitGracefully(PPM_READ_ERROR, argv[2], inputFP, outputFP);
	}
	fprintf(outputFP, "%d", colourRange);
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 
	fputc(temp, outputFP);
	

	maxSizeSupportedByImage = getSupportedImageBytes(width, height);


	/* now we can hide the message, the fail state is caught in the error variable, regardless of if the hide 
	   is successful or not we exit gracefully because it is the last thing we need to do */
	error = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);

	exitGracefully(error, argv[2], inputFP, outputFP);
}

