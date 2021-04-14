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
    /* Currently only required for the unit tests */
    #include <string.h>
    /* UNIT TEST DECLARATIONS, LATER MOVED TO SEPARATE HEADER */
    void test_push();
    void test_new_node();
    void test_pop();
    void test_file_read();
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
    void read_input(char* file_name, Node** head);
    /* Stack output */
    void print_stack(char* file_name, Node** tail);
#endif