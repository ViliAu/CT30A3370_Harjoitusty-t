/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 19.02.2021
Sources: -
*/
#ifndef REVERSE_H
#define REVERSE_H
    #include <stdio.h>
    #include <stdlib.h>
    /* Stack structure's node */
    typedef struct Node {
        char* line;
        struct Node* next;
    } Node;
    /* Dynamic memory */
    Node* new_node();
    void push(Node** node, Node** stack_head);
    Node* pop(Node** stack_head);
    void free_node(Node** node);
    /* File reading */
    char** read_lines(FILE* file_stream);
    /* File writing */
    void write(FILE* file_stream, char** lines);
#endif