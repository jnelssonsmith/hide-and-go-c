#ifndef PPMLIB_H_
#define PPMLIB_H_

int isRawPPM(FILE *inputFile, FILE *outputFile);
int getWidth(FILE *inputFP);
int getHeight(FILE *inputFP);
int getColourRange(FILE *inputFP);
void scanToNextVal(FILE *inputFile, FILE *outputFile);

#endif