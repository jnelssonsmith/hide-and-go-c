
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <SDL2/SDL.h>
#include "ppmlib.h"		// the library of functions for dealing with common ppm tasks
#include "steglib.h"	// the library of functions for hiding and revealing messages


/* 
difference for new task 1
start at 000 basename
*/
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
	
    //exitGracefully(error, outputPPM, inputFP, outputFP);
}

void multiHideMessage(int numberOfFiles, char *inputBaseName, char *outputBaseName) {
	// ./hide -m number-of-files basename outputBaseName
	fprintf(stderr, "detected: %d files, input: %s, output: %s\n", numberOfFiles, inputBaseName, outputBaseName);
	char numberString[9];
	
	int count = 0;
	while(count < numberOfFiles) {
		snprintf(numberString, 9, "-%03d.ppm", count);
		count += 1;
		char *input = malloc(strlen(inputBaseName) + strlen(numberString) + 1);
		strcpy(input, inputBaseName);
		strcat(input, numberString);
		fprintf(stderr, "%s\n", input);
	}

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
	
	int childCount = 0;

	while (!feof(inputFP)) {
		int err = fscanf(inputFP, "%s %s %s", messageFile, inputPPMName, outputPPMName);
		childCount += 1;
		pid = fork();
		if(pid == 0) {
			
			fprintf(stderr, "Spawned for line\n");
			if(err == 3) {
				FILE *messageFP = fopen(messageFile, "r");
				dup2(fileno(messageFP), STDIN_FILENO);
				execlp("./hide", "hide", inputPPMName, outputPPMName, NULL);
			}
		} else if (pid > 0) {\
		} else {
			fprintf(stderr, "Error creating process\n");
		}
	}
	fprintf(stderr, "%d children counted\n", childCount);
	int count = 0, 
		status;

	while (count < childCount) {
		waitpid(-1, &status, 0);
		count += 1;
	}
}

void drawPPMImage(int width, int height, FILE *inputFP, FILE *outputFP) {
	
	const int SCREEN_WIDTH = width * 2;
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
		window = SDL_CreateWindow("Input vs Output of Hide",
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

			int pixelCount = 0,
				y = 0,
				x = 0,
				r, 
				g, 
				b;
			while (pixelCount < (SCREEN_WIDTH * SCREEN_HEIGHT)) {
				

				// We know the first SCREEN_WIDTH/2 pixels will be input PPM and next will be outputPPM so... 
				if(x < width) {
					r = fgetc(inputFP);
					g = fgetc(inputFP);
					b = fgetc(inputFP);
				} else {
					r = fgetc(outputFP);
					g = fgetc(outputFP);
					b = fgetc(outputFP);
				}
				
				int *p = (int *)((char *)screenSurface->pixels + y * screenSurface->pitch + x * screenSurface->format->BytesPerPixel);
				*p=SDL_MapRGB(screenSurface->format, r, g, b);
				pixelCount += 1;
				x += 1;
				if(x == SCREEN_WIDTH) {
					x = 0;
					y += 1;
				}
				
			}
       
       
			SDL_UpdateWindowSurface(window);



			
		}

		/* Destroy the window */
		SDL_Event event;
            while (SDL_WaitEvent(&event)) {
              switch (event.type) {
              case SDL_QUIT:
                SDL_DestroyWindow(window);
                SDL_Quit();
                break;
              }
            }
	}

}	

void displayImage(char *inputPPM, char *outputPPM) {
	FILE *inputFP,	// the file pointer to the input ppm image
		 *outputFP;

    int temp, 					  			// used for processing chars
		height,					  				// the read height of the ppm image
		width,					  				// the read width of the ppm image
		colourRange,			  			// the read colour range of the ppm image (the range of values each pixel can take)
		error;				 

    inputFP = fopen(inputPPM, "r");
	outputFP = fopen(outputPPM, "r");

	// kill program if input is not defined
	if (inputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", inputPPM);
		exit(EXIT_FAILURE);
    }

	if (outputFP == NULL) {
		fprintf(stderr, "Could not open supplied file: %s\n", outputPPM);
		exit(EXIT_FAILURE);
    }

	
	// handle check to ensure correct PPM file type
	error = isRawPPM(inputFP, NULL);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, "", inputFP, NULL);
	}

	error = isRawPPM(outputFP, NULL);
	if(error) {
		fprintf(stderr, "Incorrect file format detected, aborting\n");
		exitGracefully(error, "", outputFP, NULL);
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

	scanToImageData(outputFP);
	
	
	// from the ppm spec we know there can only be 1 white space character between colour range and pixel raster
	temp = fgetc(inputFP); 

	drawPPMImage(width, height, inputFP, outputFP);
}

