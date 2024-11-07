#include "head.h"
#include "queue.h"


volatile int operation_count;

void *routine1() {

}

void *routine2() {

}

void *routine3() {

}

Node* generate_new_node() {
    int string_size = rand() % MAX_VALUE_SIZE;
    Node *node = malloc(sizeof(Node));
    memset(node->value, 1, string_size);
    return node;
}

int main() {
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    Node* linked_list = NULL;
    const int list_size = 10;

    srand(41148656);   
    for (int i = 0; i < list_size; ++i) {
        Node *node = generate_new_node();
        add_node(&linked_list, node);
    }

    
    Node* tmp = linked_list;
    while (tmp->next != NULL) {
        printf("%ld\n", strlen(tmp->value));
        tmp = tmp->next;
    }

    return 0;
}