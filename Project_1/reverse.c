/*
Authors: Jani Heinikoski, Vili Huusko
Last modified: 23.02.2021
Sources: -
*/
#include "reverse.h"

/* UNIT TESTS, LATER MOVED TO SEPARATE FILE */

void test_new_node() {
    Node* n = new_node();
    if (n == NULL || n->line != NULL || n->next != NULL) {
        fprintf(stderr, "error: new_node unit test failed\n");
        exit(1);
    }
}

void test_push() {
    /* NULL arg tests */
    push(NULL, NULL);
    push(NULL, new_node());
    Node* stack_head = new_node();
    push(&stack_head, NULL);
}

void test_pop() {
    Node* stack_head = new_node(), *n;
    stack_head->line = malloc(sizeof(char) * 25);
    strcpy(stack_head->line, "Original stack head");
    int i;
    char sample_text_arr[5][2] = {"1", "2", "3", "4", "5"};
    /* Push five nodes to the stack */
    for (i = 0; i < 5; i++) {
        n = new_node();
        n->line = malloc(sizeof(char) * 25);
        strcpy(n->line, sample_text_arr[i]);
        push(&stack_head, n);
    }
    /* Pop them and check that they are reversed */
    for (i = 0; i < 5; i++) {
        n = pop(&stack_head);
        if (strcmp(n->line, sample_text_arr[4 - i]) != 0) {
            fprintf(stderr, "error: test_pop unit test failed\n");
            exit(1);
        }
        free(n->line);
        free(n);
    }
    /* The original stack head should be the last one */
    n = pop(&stack_head);
    if (strcmp(n->line, "Original stack head") != 0) {
        fprintf(stderr, "error: test_pop unit test failed\n");
        exit(1);
    }
    free(stack_head->line);
    free(stack_head);

    if (pop(NULL) != NULL) {
        fprintf(stderr, "error: test_pop unit test failed\n");
        exit(1);
    }
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
        free((node->line));
    }
    free(node);
}

char** read_lines(char* file_name) {
    FILE* file_to_read = NULL;
    if ((file_to_read = fopen(file_name, "r")) == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", file_name);
        exit(1);
    }

    char* line;
    char** file_lines;
    size_t len = 0;
    ssize_t chars_read = 0;
    while ((chars_read = getline(&line, &len, file_to_read)) != -1) {
        printf("%s", line);
        realloc(file_lines, sizeof(line));
        strcpy(*file_lines, line);
    }
    fclose(file_to_read);
    return file_lines;
}

int main (int argc, char** argv) {
    read_lines("test.txt");
    getchar();
    return 0;
}