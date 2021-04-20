/*
Authors: Jani Heinikoski, Vili Huusko
Last modified (date): 20.04.2021
Sources:
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

FILE* open_file(char* file_name) {
    FILE* f = fopen(file_name, "rb");
	if (!f) {
        fprintf(stderr, "my-zip: couldn't open file\n");
        exit(1);
    }
    return f;
}

void unzip(char* file_name) {
    FILE* fptr = open_file(file_name);
    uint32_t repeated_count;
    uint8_t ascii;
    size_t read_count;
    int i;
    while ((read_count = fread(&repeated_count, 4, 1, fptr))) {
        if (read_count != 1) {
            perror("Unexpected error in fread.");
            exit(1);
        }
        read_count = fread(&ascii, 1, 1, fptr);
        if (read_count != 1) {
            perror("Unexpected error in fread.");
            exit(1);
        }

        for (i = 0; i < repeated_count; i++) {
            fprintf(stdout, "%c", ascii);
        }
    }
    fclose(fptr);
}

int main (int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        exit(1);
    }
    /* Loop the file names */
    int i = 1;
    for (; i < argc; i++) {
        unzip(argv[i]);
    }
	return 0;
}
