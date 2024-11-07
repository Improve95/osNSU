#include "head.h"
#include "queue.h"


volatile int operation_count;

void *start_routine1(void *arg) {
    Node* linked_list = (Node*) arg;

}

void *start_routine2(void *arg) {
    Node* linked_list = (Node*) arg;

}

void *start_routine3(void *arg) {
    Node* linked_list = (Node*) arg;

}

void *start_routine4(void *arg) {
    Node* linked_list = (Node*) arg;

}

void *start_routine5(void *arg) {
    Node* linked_list = (Node*) arg;

}

void *start_routine6(void *arg) {
    Node* linked_list = (Node*) arg;

}


Node* generate_new_node() {
    int string_size = rand() % MAX_VALUE_SIZE;
    Node *node = malloc(sizeof(Node));
    memset(node->value, 1, string_size);
    return node;
}

int main() {
    int err = 0;

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;

    pthread_t tid4;
    pthread_t tid5;
    pthread_t tid6;

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

    err = pthread_create(&tid1, NULL, start_routine1, linked_list);
    if (err) perror("pthread_create tid1");

    err = pthread_create(&tid2, NULL, start_routine2, linked_list);
    if (err) perror("pthread_create tid2");

    err = pthread_create(&tid3, NULL, start_routine3, linked_list);
    if (err) perror("pthread_create tid3");

    err = pthread_create(&tid4, NULL, start_routine4, linked_list);
    if (err) perror("pthread_create tid4");

    err = pthread_create(&tid5, NULL, start_routine5, linked_list);
    if (err) perror("pthread_create tid5");
    
    err = pthread_create(&tid6, NULL, start_routine6, linked_list);
    if (err) perror("pthread_create tid6");

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);

    return 0;
}