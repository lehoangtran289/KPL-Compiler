/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include "reader.h"

#include <stdio.h>

FILE *inputStream;
int lineNo, colNo;  // Current line number and column number 
int currentChar;    // Current character

// Read a character from input stream
int readChar(void) {
    currentChar = getc(inputStream);
    colNo++;
    if (currentChar == '\n') {
        lineNo++;
        colNo = 0;
    }
    return currentChar;
}

// Open input stream
int openInputStream(char *fileName) {
    inputStream = fopen(fileName, "rt");
    if (inputStream == NULL)
        return IO_ERROR;
    lineNo = 1;
    colNo = 0;
    readChar();
    return IO_SUCCESS;
}

// Close input stream
void closeInputStream() {
    fclose(inputStream);
}
