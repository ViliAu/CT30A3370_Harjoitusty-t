/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 19.02.2021
Sources: -
*/
#ifndef REVERSE_H
#define REVERSE_H
    #define _GNU_SOURCE
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <string.h>

    /* Stack structure's node */
    typedef struct Node {
        char* line;
        struct Node* next;
    } Node;
    /* Dynamic memory */
    Node* new_node();
    void push(Node**, Node*);
    Node* pop(Node**);
    void free_node(Node*, bool);

    /* Stack input */
    void read_input(char*, Node**);
    /* Stack output */
    void print_stack(char*, Node**);
    /* Open file */
    FILE* open_file(char*, char*);
    /* Get line from input */
    bool get_line(char**, FILE*);
#endif