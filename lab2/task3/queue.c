#include "head.h"
#include "queue.h"

void add_node(Node **linked_list, Node *node) {
    if (*linked_list == NULL) {
        *linked_list = node;
    } else {
        Node* tmp = *linked_list;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = node;
    }
}
