#ifndef MY_THREAD
#define MY_THREAD

#include "head.h"

typedef unsigned long int mythread_t;

typedef void *(*start_routine_t) (void *);

typedef struct {
    int                 mythread_id;
    start_routine_t     start_routine;
    void *              start_routine_arg;
    void *              ret_value;

    volatile int        joined;
    volatile int        finished;
    volatile int        detached;

    // void *              stack;
} __mythread;

int mythread_create(__mythread *newthread, void *(*start_routine) (void *), void *arg);

int mythread_join(__mythread *th, void **ret_value);

// int mythread_equals(mythread_t thread1, mythread_t thread2);

#endif
