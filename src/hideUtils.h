#ifndef HIDEUTILS_H_
#define HIDEUTILS_H_

void standardHideMessage(char *inputPPM, char *outputPPM);
void parallelExectute(char *inputFile);
void drawPPMImage(int width, int height, FILE *inputFP, FILE *outputFP);
void displayImage(char *inputFile, char *outputFile);

#endif