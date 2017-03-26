== STUDENT DETAILS ==
Josh Nelsson-Smith
25954113
Submitted for FIT3042 - System Tools And Programming Languages
26/03/2017

== INSTALLATION == 
The project includes a Makefile for convenience. 
Simply type
	$ make
or 
	$ make all

To compile both the hide and unhide programs. 
Should you want to only compile one then you can use 
	$ make hide
or 
	$ make unhide

Finally should you want to delete these files, you can use 
	$ make clean

== OVERVIEW ==
This assignment required us to create two programs, hide and unhide. hide takes in a ppm 
format image and hides a message from standard input, the program then creates a new ppm image 
with name given by the user with the message hidden within it. Unhide will take an image created by hide and 
return the secret message to standard output. 

== FILE STRUCTURE == 
/img/       - the image directory where I have put a test jpg file to hide
/ppm-files/ - the directory where you can keep some ppm files to test the program on
/src/       - where the source c files and headers reside
/test/      - for convenience I have included a test shell script that takes a ppm image and hides a jpeg within it
	      then it unhides the image. 

== USAGE == 
hide takes 2 arguments, the ppm image to hide within and the output filename, it also takes it's message from stdin
	$ ./hide somePPMImage.ppm myFileWithHiddenMessage.ppm < someInputRedirected

unhide takes 1 argument, the ppm image that has a hidden message in it and outputs the message to stdout
	$ ./unhide myFileWithHiddenMessage.ppm > someOutputRedirected

== LIMITATIONS ==
- This assignment only handles P6 (raw ppm) files

- This assignment handles any valid comment type defined in the ppm spec

- These programs enforce a colour depth value between 1 and 255

- The core functionality requested for hide and unhide is provided in this assignment

- The program has lots of error handling to handle any issues that may occur with the 
  reading and writing of PPM files, these include errors with the format string, the colour depth being
  an invalid value, errors with the sizes of messages and file i/o errors among others.

- Because the program needs to accept any message to hide, I have chosen to handle this by 
  encoding the size of the input message in a 32 bit int. This means that the max message size 
  supported by my hide function is ~2GB. I feel this is a fair tradeoff and most PPM images    	
  will not be close to having enough space to hide a message like that anyway. 

- I have not provided the bonus functionality to handle BMP format images

- You should also note that I have chosen to optimise for space over time, the entire program
  is very space effecient by making ample use of fget type calls, however as these are system 
  calls they come at the price of time. I tested the program on a variety of different file    
  sizes and didn't find the speed to be bottlenecking too much so I am happy with my choice. 

 
