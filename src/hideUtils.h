#ifndef HIDEUTILS_H_
#define HIDEUTILS_H_

void standardHideMessage(char *inputPPM, char *outputPPM);
void parallelExectute(char *inputFile);
void drawPPMImage(int width, int height, FILE *inputFP);
void displayImage(char *inputFile);

#endif