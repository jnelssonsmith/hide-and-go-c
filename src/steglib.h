#ifndef STEGLIB_H_
#define STEGLIB_H_

int getMessageSize(FILE *messageFP, char message[]);
int getNextBitToHide(int *byteIndexPtr);
int canHideMessage(int width, int height, int messageSize);
void hideMessage(int messageSize, char message[65536], FILE *inputFP, FILE *outputFP);
void hideMessageSize(int messageSize, FILE *inputFP, FILE *outputFP);
int readSizeOfSecretMessage(FILE *inputFP);
void readSecretMessage(int size, FILE *inputFP);
#endif

#ifndef FINISHED_WRITING_SECRET
#define FINISHED_WRITING_SECRET 2
#endif