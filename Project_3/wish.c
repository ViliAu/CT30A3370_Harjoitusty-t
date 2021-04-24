#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PROMPT "wish>"

#define STD_ERR_MSG "An error has occurred\n"
#define GET_LINE_ERR_MSG "Error reading from input\n"
#define INV_ARGS_ERR_MSG "Usage: ./wish [batch file]\n"
#define INV_INPUT_ERR_MSG "Invalid input\n"
#define OPEN_FILE_ERR_MSG "Could not open batch file\n"
#define BAD_BATCH_ERR_MSG "Invalid syntax in batch file\n"

void on_error(char*, bool);
bool get_line(char**, FILE*);
void interactive_mode();
bool validate_input(char*);
void batch_mode(char*);
FILE* open_file(char*, char*);

/* Wrapper with error handling for fopen */
FILE* open_file(char* filename, char* mode) {
    FILE* fp = fopen(filename, mode);
    if (!fp) {
        on_error(OPEN_FILE_ERR_MSG, true);
    }
    return fp;
}

/* Reads a whole line from input_stream to line, returns false in case of EOF */
bool get_line(char** line, FILE* input_stream) {
    if (!input_stream) {
        on_error(GET_LINE_ERR_MSG, true);
    }
    *line = NULL;
    size_t size_of_line = 0;
    ssize_t characters_read = getline(line, &size_of_line, input_stream);
    if (characters_read == -1) {
        if (ferror(input_stream) != 0) {
            /* Global errno is set if getline fails and returns -1, also returns -1 on EOF.
             According to man pages, *line should be free'd anyway. */
            on_error(GET_LINE_ERR_MSG, false);
            free(*line);
            exit(1);
        }
        free(*line);
        return false;
    }
    return true;
}

void on_error(char* error_message, bool exit_after) {
    write(STDERR_FILENO, STD_ERR_MSG, strlen(STD_ERR_MSG));
    if (error_message) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    if (exit_after) exit(1);
}

bool validate_input(char* input) {
    return true;
}

void interactive_mode() {
    char* line;
    while (true) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
        line = NULL;
        /* In either mode, if you hit the end-of-file marker (EOF), 
        you should call exit(0) and exit gracefully. */
        if (!get_line(&line, stdin)) exit(0);
        if (validate_input(line)) {
            /* Carry out the command */
        } else {
            on_error(INV_INPUT_ERR_MSG, false);
        }
    }
}

void batch_mode(char* batch_file) {
    FILE* fp = open_file(batch_file, "r");
    char* line = NULL;
    while (get_line(&line, fp)) {
        if (validate_input(line)) {
            /* Carry out the command */
        } else {
            /* Bad batch file, don't continue execution */
            on_error(BAD_BATCH_ERR_MSG, false);
            break;
        }
        line = NULL;
    }
    fclose(fp);
    /* In either mode, if you hit the end-of-file marker (EOF), 
    you should call exit(0) and exit gracefully. */
    exit(0);
}

int main(int argc, char** argv) {
    switch(argc) {
        case 1:
            interactive_mode();
            break;
        case 2:
            batch_mode(argv[1]);
            break;
        default:
            on_error(INV_ARGS_ERR_MSG, true);
            exit(1);
    }
    return 0;
}