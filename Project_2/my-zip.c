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

FILE* open_file(char*, char*);
void write_four_byte_unsigned_int(FILE*, uint32_t);
bool supported_by_ascii(int);
void check_src_dest(FILE*, FILE*);
void zip(FILE*, FILE*);


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
void write_four_byte_unsigned_int(FILE* dest, uint32_t four_byte_uint) {
    if (!dest) {
        fprintf(stderr, "Destination was null.\n");
        exit(1);
    }
    size_t elems_written = fwrite(&four_byte_uint, 4, 1, dest);
    if (elems_written != 1) {
        /* Perror used because global errno was set by fwrite */
        perror("Error in writing to file in write_four_byte_unsigned_int.");
        exit(1);
    }
}

bool supported_by_ascii(int c) {
    return (c >= 0 && c <= 127);
}

void insert_if_supported(FILE* dest, int val, uint32_t rc) {
    if (supported_by_ascii(val)) {
        /*write_four_byte_unsigned_int(dest, rc);*/
        puts("Supported");
    } else {
        fprintf(stderr, "Encountered a non-ASCII supported character: %c, omitting...\n", val);
    }
}

void check_src_dest(FILE* src, FILE* dest) {
    if (!src && dest) {
        fclose(dest);
        fprintf(stderr, "Src arg was NULL.\n");
    } else if (src && !dest) {
        fclose(src);
        fprintf(stderr, "Dest arg was NULL.\n");
    } else if (!src && !dest) {
        fprintf(stderr, "Src and dest args were NULL.\n");
    } else {
        return;
    }
    exit(1);
}

void zip(FILE* src, FILE* dest) {
    check_src_dest(src, dest);
    int rc, prev;
    uint32_t repeatc = 1;
    if ((prev = fgetc(src)) != EOF) {
        while ((rc = fgetc(src)) != EOF) {
            /* Platform independent maximum value of 32-bit unsigned int, required in case overflow */
            if (rc == prev && repeatc < 4294967295) {
                repeatc++;
                continue;
            }
            insert_if_supported(dest, prev, rc);
            putc(prev, dest); /* TODO For testing purposes, remove on deployment */
            printf("\nrepeated: %d\n", repeatc);
            repeatc = 1;
            prev = rc;
        }
        insert_if_supported(dest, prev, rc);
        putc(prev, dest); /* TODO For testing purposes, remove on deployment */
        printf("\nrepeated: %d\n", repeatc);
    }
    if (ferror(src) != 0) {
        perror("I/O Error in zip.");
        exit(1);
    } 
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        puts("Usage: my-zip: file1 [file2 ...]");
        exit(1);
    } else {
        for (argv++; *argv != NULL; argv++) {
            zip(open_file(*argv, "r"), stdout);
        }
    }
    return 0;
}