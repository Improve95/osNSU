#ifndef QUEUE_H
#define QUEUE_H

#include "head.h"

#define MAX_VALUE_SIZE 100

typedef struct _Node {
    char value[MAX_VALUE_SIZE];
    struct _Node *next;
    pthread_mutex_t lock;
} Node;

void add_node(Node **linked_list, Node *node);

#endif
