
#include "input.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

unsigned char END_OF_FILE = 255;
struct input Input;


void ReadSourceFile(char *filename) {
    /************************************************************
            Use standard C I/O library to access file.      (*.i files)
     ************************************************************/
    int len;

    Input.file = fopen(filename, "r");
    if (Input.file == NULL) { 
        Fatal("Can't open file: %s.", filename);
    }
    /***********************************************************
    The ftell() function obtains the current value of the file position
    indicator for the stream pointed to by stream.
    fseek + ftell ---->  file size

     ***********************************************************/
    fseek(Input.file, 0, SEEK_END);
    Input.size = ftell(Input.file);
    // allocate enough heap memory.
    Input.base = (unsigned char *)malloc(Input.size + 1);
    if (Input.base == NULL) {
        Fatal("The file %s is too big", filename);
        fclose(Input.file);
    }
    // set current position to the beginning of the file.
    fseek(Input.file, 0, SEEK_SET);
    Input.size = fread(Input.base, 1, Input.size, Input.file);
    fclose(Input.file);

    return;
}





void FreeSource(void) {
    free(Input.base);
}

