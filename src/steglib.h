 /*
NAME: Joshua Nelsson-Smith
START DATE: 19/03/17
LAST MODIFIED: 24/03/17
DESCRIPTION: The steganography library defines a group of functions that are useful
when dealing with the hiding and reading of secret messages in files.
*/
#ifndef STEGLIB_H_
#define STEGLIB_H_


/*
getSupportedImageBytes is a function returns the number of available bytes the given image has to hide within. 
To calculate the number of bytes in the image we can hide within we only need to take in the width and height of 
the image as inputs. Because an image is composed of pixel values the max number of bytes to hide within is 
width x height x 3.  
*/
int getSupportedImageBytes(int width, int height);

/*
canHideMessage is a function that tells you whether or not a given message will fit in a
ppm file based on the width and height defined in the file. A file will have width x height 
pixels to hide in. 
If the message can fit it will return MESSAGE_WRITE_SUCCESS, otherwise it will return MESSAGE_WRITE_ERROR.
The inputs are the size of the message you want to hide in bytes and the 
max size supported by the image
*/
int canHideMessage(int messageSize, int maxSizeSupportedByImage);

/*
hideMessage is a function that hides a given message inside the rgb values of a given pixel raster and writes these 
manipulated values into an output file.
It splits up the input message into chars, and each char into the 8 bits that represent a char, and hides 
each of those bits inside the last bit of an r,g or b value. 
This function does not returns either a MESSAGE_WRITE_ERROR or MESSAGE_WRITE_SUCCESS
The function takes message size as an input so it knows when to stop writing the secret, 
it also takes in the message to write, an input file to read the rgb values to alter and an output file pointer 
to write the values to. 
*/
int hideMessage(int maxSizeSupportedByImage, FILE *inputFP, FILE *outputFP, int multiMode, int numberOfFiles);

/*
hideMessageSize is very similar to hide message, but it is required to be run before we start hiding the message inside the 
values because an unhide function needs to know when to stop looking for the message. It essentially hides the 
the size of the hidden message (in bytes) as a 16 bit integer. This is required because we can not limit he types of 
input we can hide, so we cannot rely on using null bytes to know when to stop.
The function does not return anything. 
The function takes in the message size, the input file pointer to read from and the output file pointer to write to
*/
void hideMessageSize(int messageSize, FILE *inputFP, FILE *outputFP);

/*
readSizeOfSecretMessage reads out the first 16 bits worth of info from an image with a hidden message inside 
and converts it to an integer representing the amount of bytes of info hidden within the image.
The function returns the size as read from the hidden message image.
The function takes only the input file pointer as input. 
*/
int readSizeOfSecretMessage(FILE *inputFP);

/*
readSecretMessage reads out the secret message hidden within an image and outputs it to stdout
The function does not return anything but does output the secret message.
The functon takes in the size of the secret message and the input file pointer to read it from as 
inputs
*/
void readSecretMessage(int size, FILE *inputFP);

int hideNumberOfFiles(int numberOfFiles, FILE *inputFP, FILE *outputFP);

#endif


// These two definitions make the code more readable and can be used for signalling to the calling functions 
#ifndef MESSAGE_WRITE_SUCCESS
#define MESSAGE_WRITE_SUCCESS	0
#endif

#ifndef MESSAGE_WRITE_ERROR
#define MESSAGE_WRITE_ERROR		1
#endif
