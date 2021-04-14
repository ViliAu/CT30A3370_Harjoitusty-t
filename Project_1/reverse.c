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

void test_file_read() {
    Node* head = new_node();
    Node* n;
    read_input("test_original.txt", &head);
    while ((n = pop(&head)) != NULL) {
        if(n->line == NULL)
            break;
        fprintf(stdout, "%s", n->line);
    }
}

/* END UNIT TESTS */


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

/* Reads the lines from an input (file) and pushes them to the stack */
void read_input(char* file_name, Node** head) {
    char* line;
    Node* node;
    size_t len = 0;
    ssize_t chars_read = 0;
    /* Choose the file to read based on the file name */
    FILE* file_to_read;
    if (strcmp(file_name, "stdin")) {
        if ((file_to_read = fopen(file_name, "r")) == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", file_name);
            exit(1);
        }
    }
    else {
        file_to_read = stdin;
    }
    while ((chars_read = getline(&line, &len, file_to_read)) != -1) {
        /* Break user input on an empty line */
        if (!strcmp(file_name, "stdin") && !strcmp(line, "\n"))
            break;
        node = new_node();
        node->line = malloc(sizeof(char) * sizeof(line));
        strcpy(node->line, line);
        push(head, node);
    }
    free(line);
    /* Close the file if not stdin */
    if (strcmp(file_name, "stdin"))
        fclose(file_to_read);
}



void print_stack(char* file_name, Node** tail) {
    Node* node;
    /* Handle output file */
    FILE* out;
    if (strcmp(file_name, "stdout")) {
        if ((out= fopen(file_name, "w")) == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", file_name);
            exit(1);
        }
    }
    else {
        out = stdout;
    }
    while((node = pop(tail)) != NULL) {
        if (node->line == NULL)
            break;
        fprintf(out, "%s", node->line);
    }
    if (strcmp(file_name, "stdin"))
        fclose(out);
}

int main (int argc, char** argv) {
    Node* head = new_node();
    char* file_name = ((argc < 2) ? "stdin" : argv[1]);
    read_input(file_name, &head);
    file_name = ((argc < 3) ? "stdout" : argv[2]);
    print_stack(file_name, &head);
    return 0;
}