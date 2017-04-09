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

struct arguments {
  int numberOfFiles;      		  /* Argument for -m */
  char *instructionFile;    		/* Argument for -p */
  int sideBySide; 							/* Indicates precence of -s flag */
	char *inputPPM;
	char *outputPPM;
};

#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages
#include "hideUtils.h" 

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

			if(state->arg_num == 0) {
				 arguments->inputPPM = arg;
			} else if (state->arg_num == 1) {
				 arguments->outputPPM = arg;
			}
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
	
	// set program defaults here
	arguments.sideBySide = 0;
	arguments.numberOfFiles = 1; 										// by default number of files message can be written across is 1
	arguments.instructionFile = "(notSupplied)";
	arguments.inputPPM = "(notSupplied)";
	arguments.outputPPM = "(notSupplied)";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	printf("%s\n", arguments.inputPPM);
	printf("%s\n", arguments.outputPPM);

	switch(flagType) {
		case 's':
			fprintf(stderr, "s flag detected\n");
			break;
		case 'p':
			fprintf(stderr, "p flag detected\n");
			break;
		case 'm':
			fprintf(stderr, "m flag detected\n");
			break;
		case '0':
			fprintf(stderr, "no flags detected\n");
			standardHideMessage(arguments);
			break;
	
		default:
			printf("Too many flags detected\nHide only supports 1 flag at a time or no flags\n");
			exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

