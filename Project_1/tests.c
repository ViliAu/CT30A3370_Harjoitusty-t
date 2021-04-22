
#include <string.h>

void test_push();
void test_new_node();
void test_pop();
void test_file_read();

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