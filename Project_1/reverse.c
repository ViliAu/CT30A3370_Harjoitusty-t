/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 19.02.2021
Sources: -
*/
#include "reverse.h"

Node* new_node() {
    /* Used to malloc Nodes for the linked list */
    Node* n = malloc(sizeof(Node));

    if (n == NULL) {
        fprintf(stderr, "error: malloc failed");
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
    Node* tmp = *stack_head;
    node->next = *stack_head;
    *stack_head = node;
}

void free_node(Node* node, bool line_needs_free) {
    /* Free's the malloc'd node */
    if (line_needs_free == true) {
        free(node->line);
    }
    free(node);
}

int main (void) {
    return 0;
}