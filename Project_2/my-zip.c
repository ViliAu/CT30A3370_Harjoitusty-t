/*
Authors: Jani Heinikoski, Vili Huusko
Last modified (date): 15.04.2021
Sources:
- fwrite man page
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define READ_BINARY "rb"
#define WRITE_BINARY "wb"

/* Wrapper with error handling for fopen */
FILE* open_file(char* filename, char* mode) {
    FILE* fp = fopen(filename, mode);
    if (!fp) {
        fprintf(stderr, "Error opening the file: %s with mode %s\n", filename, mode);
        exit(1);
    }
    return fp;
}

bool supported_by_ascii(int c) {
    return (c >= 0 && c <= 127);
}