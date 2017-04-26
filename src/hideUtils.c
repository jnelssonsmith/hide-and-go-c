
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <SDL2/SDL.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages



void standardHideMessage(char *inputPPM, char *outputPPM) {
    FILE *inputFP,	// the file pointer to the input ppm image
	     *outputFP; // the file pointer to the output ppm image with the hidden message inside

    int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				 

    inputFP = fopen(inputPPM, "rb");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", inputPPM);
		exit(EXIT_FAILURE);
    }
	
	outputFP = fopen(outputPPM, "wb");
	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, outputFP);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, outputPPM, inputFP, outputFP);
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
		exitGracefully(PPM_READ_ERROR, outputPPM, inputFP, outputFP);
	}
	
    fprintf(outputFP, "%d", colourRange);
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 
	fputc(temp, outputFP);
	
	maxSizeSupportedByImage = getSupportedImageBytes(width, height);
	/* now we can hide the message, the fail state is caught in the error variable, regardless of if the hide 
		is successful or not we exit gracefully because it is the last thing we need to do */
	error = hideMessage(maxSizeSupportedByImage, inputFP, outputFP);
	
    exitGracefully(error, outputPPM, inputFP, outputFP);
}

void parallelExectute(char *inputFile) {
	FILE *inputFP;
	pid_t pid;

	inputFP = fopen(inputFile, "r");
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", inputFile);
		exit(EXIT_FAILURE);
    }

	// we'll define a max string length of 20 chars
	char messageFile[24],
		 inputPPMName[24],
		 outputPPMName[24];
	while (!feof(inputFP)) {
		int err = fscanf(inputFP, "%s %s %s", messageFile, inputPPMName, outputPPMName);
		pid = fork();
		if(pid == 0) {
			fprintf(stderr, "Spawned for line\n");
			if(err == 3) {
				FILE *messageFP = fopen(messageFile, "r");
				dup2(fileno(messageFP), STDIN_FILENO);
				execlp("./hide", "hide", inputPPMName, outputPPMName, NULL);
			}
			exit(0);
		} else if (pid > 0) {
			
		} else {
			fprintf(stderr, "Error creating process\n");
		}
	}
}

void drawPPMImage(int width, int height, FILE *inputFP) {
	
	const int SCREEN_WIDTH = width;
	const int SCREEN_HEIGHT = height;

	/* The window to render to */
	SDL_Window *window = NULL;

	/* The surface contained by the window */
	SDL_Surface *screenSurface = NULL;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	else {
		/* Create the window */
		window = SDL_CreateWindow("Hello Pixel",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			/* Get screen surface */
			screenSurface = SDL_GetWindowSurface(window);

			/* Fill surface with white */
			SDL_FillRect(screenSurface, NULL, 
				SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

			int pixelCount = 0;
			int r, g, b;

			while (!feof(inputFP)) {
				r = fgetc(inputFP);
				g = fgetc(inputFP);
				b = fgetc(inputFP);

				

				if(r > -1 & g > -1 & b > -1) {
					int *p = (int *)screenSurface->pixels + pixelCount;
					*p=SDL_MapRGB(screenSurface->format, r, g, b);
					pixelCount += 1;
				}
				
			}
       
			SDL_UpdateWindowSurface(window);



			/* Wait two seconds */
			SDL_Delay(4000); 
		}

		/* Destroy the window */
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

}	

void displayImage(char *inputPPM) {
	FILE *inputFP;	// the file pointer to the input ppm image

    int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		maxSizeSupportedByImage,  // the number of bytes there is in the ppm image to hide a message within. 
		error;				 

    inputFP = fopen(inputPPM, "r");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", inputPPM);
		exit(EXIT_FAILURE);
    }
	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, NULL);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, "", inputFP, NULL);
	}
	
	// scan through width, heigh and colour range values
	scanToNextVal(inputFP, NULL);
	
	width = getWidth(inputFP);
	
	scanToNextVal(inputFP, NULL);
	
	height = getHeight(inputFP);

	scanToNextVal(inputFP, NULL);
	
	colourRange = getColourRange(inputFP);
	
    if(colourRange == PPM_COLOUR_READ_ERROR) {
		exitGracefully(PPM_READ_ERROR, "", inputFP, NULL);
	}
	
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 

	drawPPMImage(width, height, inputFP);
}

