#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int createThreadPool(int count, void *runnable, int *threadsId, pthread_t **poolThreads);

int joinThreadPool(pthread_t *poolThreads, int sizeThreadPool);

int cancelThreadPool(pthread_t *poolThreads, int sizeThreadPool);

#endif //THREADPOOL_H
