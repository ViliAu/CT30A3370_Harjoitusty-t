/*
Authors: Jani Heinikoski, Vili Huusko
Last modified (date): 19.04.2021
Sources:
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void unzip(char* file_name) {
    FILE* fptr = fopen(file_name, "rb");
	if (!fptr) {
        fprintf(stderr, "my-zip: couldn't open file\n");
        exit(1);
    }
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
    int i = 2;
    for (; i < argc; i++) {
        unzip(argv[i]);
    }
	return 0;
}
