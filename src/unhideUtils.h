#ifndef UNHIDEUTILS_H_
#define UNHIDEUTILS_H_

void unhideMessage(FILE *inputFP, int multiMode);
FILE* getUnhideFP(char *inputName, int multiMode); 
int getNumberOfFiles(FILE *inputFP);

#endif