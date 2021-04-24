#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define STD_ERR_MSG "An error has occurred\n"
#define INV_ARGS_ERR_MSG "Usage: ./wish [batch file]\n"

void on_error(char*, bool);

void on_error(char* error_message, bool exit_after) {
    write(STDERR_FILENO, STD_ERR_MSG, strlen(STD_ERR_MSG));
    if (error_message) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    if (exit_after) exit(1);
}

int main(int argc, char** argv) {
    switch(argc) {
        case 1:
            /* interactive mode */
            break;
        case 2:
            /* batch mode */
            break;
        default:
            on_error(INV_ARGS_ERR_MSG, true);
            exit(1);
    }
    return 0;
}