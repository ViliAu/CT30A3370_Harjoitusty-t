/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 25.04.2021
Sources: -
*/
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
#define MALLOC_ERR_MSG "Failed to allocate memory\n"

#define BUILT_IN_EXIT "exit" 
#define BUILT_IN_PATH "path"
#define BUILT_IN_CD "cd"

struct Token {
    char* token;
    size_t token_length;
    struct Token* next;
} typedef Token;

void on_error(char*, bool);
bool get_line(char**, FILE*);
void interactive_mode();
bool validate_input(char*, Token**);
void batch_mode(char*);
FILE* open_file(char*, char*);
Token* new_token();
void free_list(Token*);
char* new_str(size_t);
void increment_str_size(char**);
void tokenize(Token**, char*);
Token* tokenize_further(char**, size_t);
void add_token(Token**, Token*);
bool supported_by_ascii(int);
bool check_multiple_redirs(char*, size_t);
bool check_repeating_ampersands(char*, size_t);
Token* new_redirect_token();
void exit_shell(bool);

Token* PATH = NULL;

/* ****************************** TEST FUNCTIONS ****************************** */
/* For test purposes only */
void print_list(Token*);
/* For test purposes only */
void print_list(Token* head) {
    Token* ptr = head;
    while (ptr) {
        printf("token: %s, token_length: %ld\n", ptr->token, ptr->token_length);
        ptr = ptr->next;
    }
}
/* ****************************** UTILITY FUNCTIONS ****************************** */

void exit_shell(bool erroneous_exit) {
    if (PATH)
        free_list(PATH);
    exit(erroneous_exit);
}

/* Called whenever an error is detected, prints default err msg and arg error_message to stderr */
void on_error(char* error_message, bool exit_after) {
    write(STDERR_FILENO, STD_ERR_MSG, strlen(STD_ERR_MSG));
    if (error_message) {
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    if (exit_after) exit_shell(1);
}

bool check_multiple_redirs(char* input, size_t input_length) {
    size_t i = 0;
    unsigned short counter = 0;
    for (; i < (input_length - 1); i++) {
        if (*(input + i) == '>')
            counter++;
    }
    return (counter > 1);
}

bool check_repeating_ampersands(char* input, size_t input_length) {
    size_t i = 0;
    for (; i < (input_length - 1); i++) {
        if (*(input + i) == '&' && *(input + (i + 1)) == '&')
            return true;
    }
    return false;
}

/* Validates read input and divides it into tokens to a list defined by head,
 does not guarantee correct commands but checks for syntax errors */
bool validate_input(char* input, Token** head) {
    if (!input) return false;
    size_t input_length = strlen(input);
    if (input_length == 0) return false;
    /* Only allowed to have one > -symbol per command */
    if (check_multiple_redirs(input, input_length)) return false;
    /* Only allowed to have one repeating & -symbol per command */
    if (check_repeating_ampersands(input, input_length)) return false;
    char* left_token = strtok(input, ">"), *right_token = strtok(NULL, ">");
    if (!left_token) return false;
    if (right_token) {
        tokenize(head, left_token);
        if (*head == NULL) return false;
        add_token(head, new_redirect_token());
        tokenize(head, right_token);
    } else {
        tokenize(head, input);
    }
    if (*head == NULL) return false;
    if (*(*head)->token == '>') return false;
    return true;
}

bool supported_by_ascii(int c) {
    return (c >= 0 && c <= 127);
}

/* Returns a token with &\0 as its default token property */
Token* new_ampersand_token() {
    Token* tok = new_token();
    tok->token = new_str(2);
    tok->token_length = 2;
    strcpy(tok->token, "&\0");
    return tok;
}

/* Returns a token with >\0 as its default token property */
Token* new_redirect_token() {
    Token* tok = new_token();
    tok->token = new_str(2);
    tok->token_length = 2;
    strcpy(tok->token, ">\0");
    return tok;
}

/* Tokenizes the given token based on ampersands (because they do not require whitespace) */
Token* tokenize_further(char** token, size_t token_len) {
    Token* head = NULL, *ptr = NULL;
    bool add_ampersand_to_end = (*(*token + (token_len - 2)) == '&');
    char* tok = strtok(*token, "&");
    size_t tok_length;
    while (tok != NULL) {
        if (**token == '&')
            add_token(&head, new_ampersand_token());
        tok_length = strlen(tok);
        ptr = new_token();
        ptr->token = new_str(tok_length + 1);
        ptr->token_length = tok_length + 1;
        strcpy(ptr->token, tok);
        add_token(&head, ptr);
        if ((tok = strtok(NULL, "&")) != NULL) {
            add_token(&head, new_ampersand_token());
        }
    }
    if (add_ampersand_to_end)
        add_token(&head, new_ampersand_token());
    return head;
}

/* Splits the line into a token list (used to parse commands) */
void tokenize(Token** head, char* line) {
    size_t line_length = strlen(line), i = 0, j;
    char* temp_tok;
    bool token_found;
    for (; i < line_length; i++) {
        token_found = false;
        j = 0;
        /* Only 7-bit ascii-characters are supported  */
        if (!supported_by_ascii(*(line + i))) {
            free_list(*head);
            *head = NULL;
            return;
        }
        temp_tok = new_str(1);
        *temp_tok = '\0';
        while (i <= line_length && *(line + i) != ' ' && *(line + i) != '\t' && *(line + i) != '\n' && *(line + i) != '\0') {
            /* Only 7-bit ascii-characters are supported  */
            if (!supported_by_ascii(*(line + i))) {
                free_list(*head);
                free(temp_tok);
                *head = NULL;
                return;
            }
            increment_str_size(&temp_tok);
            temp_tok[j++] = *(line + i++);
            temp_tok[j] = '\0';
            token_found = true;
        }
        if (token_found) {
            Token* token;
            if ((token = tokenize_further(&temp_tok, (j + 1)))) {
                add_token(head, token);
                Token* ptr = *head;
                while (ptr->next) {
                    if (*ptr->token == '&' && *ptr->next->token == '&') {
                        free_list(*head);
                        free(temp_tok);
                        *head = NULL;
                        return;
                    }
                    ptr = ptr->next;
                }
            } else {
                token = new_token();
                token->token = new_str(j + 1);
                strcpy(token->token, temp_tok);
                token->token_length = j + 1;
                add_token(head, token);
            }
        }
        free(temp_tok);
    }
}

/* ****************************** DYNAMIC MEMORY RELATED FUNCTIONS ****************************** */

/* Returns a new token with default initialized values */
Token* new_token() {
    Token* token = malloc(sizeof(Token));
    if (!token) {
        on_error(MALLOC_ERR_MSG, true);
    }
    token->token_length = 0;
    token->token = NULL;
    token->next = NULL;
    return token;
}

/* Free's the linked list defined by head */
void free_list(Token* head) {
    Token* ptr = head;
    while(ptr) {
        head = ptr;
        ptr = ptr->next;
        free(head->token);
        free(head);
    }
}

/* Malloc's and returns a ptr to string of size str_length in bytes */
char* new_str(size_t str_length) {
    char* str = malloc(str_length);
    if (!str) {
        on_error(MALLOC_ERR_MSG, false);
        exit_shell(1);
    }
    return str;
}

/* Increments the argument string's size in bytes by one. Requires null byte at the end of str! */
void increment_str_size(char** str) {
    size_t new_size = strlen(*str) + 2;
    *str = realloc(*str, new_size);
    if (!(*str)) {
        on_error(MALLOC_ERR_MSG, false);
        exit_shell(1);
    }
}

/* Adds the given token tok to list defined by head */
void add_token(Token** head, Token* tok) {
    if (*head == NULL) {
        *head = tok;
        return;
    }
    Token* ptr = *head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = tok;
}

/* ****************************** FILE I/O FUNCTIONS ****************************** */

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
            exit_shell(1);
        }
        free(*line);
        return false;
    }
    return true;
}

/* ****************************** SHELL MODES ****************************** */

void interactive_mode() {
    char* line;
    Token* head = NULL;
    while (true) {
        write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
        head = NULL;
        line = NULL;
        /* In either mode, if you hit the end-of-file marker (EOF), 
        you should call exit(0) and exit gracefully. */
        if (!get_line(&line, stdin)) exit_shell(0);
        /* validate user input */
        if (validate_input(line, &head)) {
            print_list(head); /* Test function */
            if (strcmp(head->token, BUILT_IN_EXIT) == 0) {
                break;
            }
        } else {
            on_error(INV_INPUT_ERR_MSG, false);
        }
        /* Line and its tokens need to be free'd after each command */
        free(line);
        free_list(head);
    }
    free(line);
    free_list(head);
    exit_shell(0);
}

void batch_mode(char* batch_file) {
    FILE* fp = open_file(batch_file, "r");
    Token* head = NULL;
    char* line = NULL;
    while (get_line(&line, fp)) {
        if (validate_input(line, &head)) {
            print_list(head); /* Test function */
            if (strcmp(head->token, BUILT_IN_EXIT) == 0) {
                break;
            }
        } else {
            /* Bad batch file, don't continue execution */
            on_error(BAD_BATCH_ERR_MSG, false);
            break;
        }
        free(line);
        free_list(head);
        head = NULL;
        line = NULL;
    }
    free(line);
    free_list(head);
    fclose(fp);
    /* In either mode, if you hit the end-of-file marker (EOF), 
    you should call exit(0) and exit gracefully. */
    exit_shell(0);
}

int main(int argc, char** argv) {
    add_token(&PATH, new_token());
    PATH->token = new_str(5);
    strcpy(PATH->token, "/bin");
    PATH->token_length = 5;
    switch(argc) {
        case 1:
            interactive_mode();
            break;
        case 2:
            batch_mode(argv[1]);
            break;
        default:
            on_error(INV_ARGS_ERR_MSG, true);
            exit_shell(1);
    }
    return 0;
}