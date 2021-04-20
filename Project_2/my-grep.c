/*
Authors: Jani Heinikoski, Vili Huusko
Last modified (date): 19.04.2021
Sources:
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Change the get_line func to the wrapper */
void search_word(char* word, char* file_name) {
    char* line;
    size_t len = 0;
    ssize_t chars_read = 0;

    /* Open file */
    FILE* file_to_read;
    if (strcmp(file_name, "stdin")) {
        if ((file_to_read = fopen(file_name, "r")) == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", file_name);
            exit(1);
        }
    }
    else {
        file_to_read = stdin;
    }
    /* i is used to loop through the read line */
    /* j is used to check the same characters */
    int i, j;
    /* Current char in the word we're trying to find */
    while ((chars_read = getline(&line, &len, file_to_read)) != -1) {
        /* Break user input on an empty line */
        if (!strcmp(file_name, "stdin") && !strcmp(line, "\n"))
            break;
        j = 0;
        for (i = 0; i < chars_read; i++) {
            if (line[i] == word[j]) {
                j++;
            }
            else {
                j = 0;
            }
            /* Check if the word has been found */
            if (j == strlen(word)) {
                fprintf(stdout, "%s", line);
                break;
            }
        }
    }
    free(line);
    /* Close the file if not stdin */
    if (strcmp(file_name, "stdin"))
        fclose(file_to_read);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "my-grep: searchterm [file...]\n");
        exit(1);
    }
    /* Loop the file names */
    if (argc == 2) {
        search_word(argv[1], "stdin");
    }
    else {
        int i = 2;
        for (; i < argc; i++) {
            search_word(argv[1], argv[i]);
        }
    }
    return 0;
}
