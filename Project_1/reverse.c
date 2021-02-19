/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 19.02.2021
Sources: -
*/
#include "reverse.h"

/* UNIT TESTS, LATER MOVED TO SEPARATE FILE */

void test_new_node() {
    Node* n = new_node();
    if (n == NULL || n->line != NULL || n->next != NULL) {
        fprintf(stderr, "error: new_node unit test failed\n");
    }
}

void test_push() {
    /* NULL arg tests */
    push(NULL, NULL);
    push(NULL, new_node());
    Node* stack_head = new_node();
    push(&stack_head, NULL);
}

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

void free_node(Node* node, bool line_needs_free) {
    /* Free's the malloc'd node */
    if (node == NULL) {
        return;
    }
    if (line_needs_free == true && node->line != NULL) {
        free((node->line));
    }
    free(node);
}

int main (void) {
    return 0;
}