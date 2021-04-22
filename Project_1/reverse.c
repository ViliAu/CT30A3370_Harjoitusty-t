/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 22.04.2021
Sources: -
*/
#include "reverse.h"

/* Uncomment to include unit tests */
/* #include "tests.c" */

Node* new_node() {
    /* Used to malloc Nodes for the linked list */
    Node* n = malloc(sizeof(Node));

    if (n == NULL) {
        fprintf(stderr, "error: malloc failed\n");
        exit(1);
    }

    n->line = NULL;
    n->next = NULL;

    return n;
}

void push(Node** stack_head, Node* node) {
    /* Pushes a new node to the stack */
    if (stack_head == NULL || node == NULL) {
        return;
    }
    node->next = *stack_head;
    *stack_head = node;
}

Node* pop(Node** stack_head) {
    /* Removes and returns the last pushed node from the stack,
        modifies stack_head argument to match the new head. */
    if (stack_head == NULL || *stack_head == NULL) {
        return NULL;
    }
    if ((*stack_head)->next == NULL) {
        return *stack_head;
    }
    /* Move the stack head to the second last element and return last pushed (remove its link to the stack) */
    Node* new_head = (*stack_head)->next;
    Node* popped_node = (*stack_head);
    (*stack_head)->next = NULL;
    (*stack_head) = new_head;
    return popped_node;
}

void free_node(Node* node, bool line_needs_free) {
    /* Free's the malloc'd node */
    if (node == NULL) {
        return;
    }
    if (line_needs_free == true && node->line != NULL) {
        free(node->line);
    }
    free(node);
}

FILE* open_file(char* file_name, char* mode) {
    FILE* fs;
    /* If file name was passed, try to open a file with the passed file_name value and mode */
    if (file_name) {
        if ((fs = fopen(file_name, mode)) == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", file_name);
            exit(1);
        }
    }
    /* If file name is not passed, return stdin or stdout depending on the mode */
    else {
        if (mode[0] == 'r') {
            fs = stdin;
        }
        else {
            fs = stdout;
        }
    }
    return fs;
}

bool get_line(char** line, FILE* input_stream) {
    if (!input_stream) {
        fprintf(stderr, "error: input stream was NULL\n");
        exit(1);
    }
    *line = NULL;
    size_t size_of_line = 0;
    ssize_t characters_read = getline(line, &size_of_line, input_stream);
    if (characters_read == -1) {
        if (ferror(input_stream) != 0) {
            /* Global errno is set if getline fails and returns -1, also returns -1 on EOF.
             According to man pages, *line should be free'd anyway. */
            fprintf(stderr, "error: getline");
            free(*line);
            exit(1);
        }
        return false;
    }
    return true;
}

/* Reads the lines from an input (file) and pushes them to the stack */
void read_input(char* file_name, Node** head) {
    char* line = NULL;
    Node* node;

    /* Choose the file to read based on the file name */
    FILE* file_to_read = open_file(file_name, "r");
    while (get_line(&line, file_to_read)) {
        /* Break user input on an empty line */
        if (!file_name && line[0] == '\n')
            break;
        node = new_node();
        node->line = malloc(strlen(line)+1); /* 1 extra byte for null-char */
        if (node->line == NULL) {
            fprintf(stderr, "error: malloc failed\n");
            exit(1);
        }
        strcpy(node->line, line);
        push(head, node);
        free(line);
    }
    free(line);
    /* Close the file if not stdin */
    if (file_name)
        fclose(file_to_read);
}

void print_stack(char* file_name, Node** tail) {
    Node* node;
    /* Handle output file */
    FILE* out = open_file(file_name, "w");
    while((node = pop(tail)) != NULL) {
        if (node->line == NULL)
            break;
        fprintf(out, "%s", node->line);
        free_node(node, true);
    }
    if (file_name)
        fclose(out);
}

int main (int argc, char** argv) {
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }
    if (argc == 3) {
        if (!strcmp(argv[1], argv[2])) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
    }
    Node* head = new_node();
    char* file_name = ((argc < 2) ? NULL : argv[1]);

    read_input(file_name, &head);
    file_name = ((argc < 3) ? NULL : argv[2]);
    print_stack(file_name, &head);
    free_node(head, false);
    return 0;
}
