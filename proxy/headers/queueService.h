#ifndef QUEUESERVICE_H
#define QUEUESERVICE_H

#include <pthread.h>
#include "pthreadService.h"

struct entry {
    int socket;
    struct Entry *next;
} typedef Entry;

struct queue {
    int size;

    Entry *head;
    Entry *tail;

    pthread_mutex_t queueMutex;
    pthread_cond_t condVar;

} typedef Queue;

Queue *createQueue();

int getSocketFromQueue(Queue *queue);

void putSocketInQueue(Queue *queue, int sock);

int isEmpty(const Queue *queue);

#endif //QUEUESERVICE_H
