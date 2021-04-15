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

/* Writes the four_byte_uint into dest as a four byte block in binary format.
    FILE* must be in wb mode (e.g., get FILE* using fopen() beforehand) */
void write_unsigned_int(FILE* dest, uint32_t four_byte_uint) {
    if (!dest) {
        fprintf(stderr, "Destination was null.\n");
        exit(1);
    }
    size_t elems_written = fwrite(&four_byte_uint, 4, 1, dest);
    if (elems_written != 1) {
        /* Perror used because global errno was set by fwrite */
        perror("Error in writing to file in write_unsigned_int.");
        exit(1);
    }
}

bool supported_by_ascii(int c) {
    return (c >= 0 && c <= 127);
}