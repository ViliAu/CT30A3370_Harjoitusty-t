/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 19.02.2021
Sources: -
*/
#ifndef REVERSE_H
#define REVERSE_H
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <string.h>
    /* UNIT TEST DECLARATIONS, LATER MOVED TO SEPARATE FILE */
    void test_push();
    void test_new_node();
    void test_pop();
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
    /* File reading */
    char** read_lines(FILE*);
    /* File writing */
    void write(FILE*, char**);
#endif