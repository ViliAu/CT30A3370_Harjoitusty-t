#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search_word(char* word, char* file_name) {
    char* line;
    size_t len = 0;
    ssize_t chars_read = 0;

    /* Open file */
    FILE* file_to_read;
    if ((file_to_read = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'\n", file_name);
        return;
    }
    /* i is used to loop through the read line */
    /* j is used to check the same characters */
    int i, j;
    /* Current char in the word we're trying to find */
    while ((chars_read = getline(&line, &len, file_to_read)) != -1) {
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
    fclose(file_to_read);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        exit(1);
    }
    /* Loop the file names */
    int i = 2;
    for (; i < argc; i++) {
        search_word(argv[1], argv[i]);
    }
    return 0;
}
