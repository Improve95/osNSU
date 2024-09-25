#ifndef MY_THREAD
#define MY_THREAD

#include "head.h"

typedef void *(*start_routine_t) (void *);

typedef struct {
    int                 mythread_id;
    start_routine_t     start_routine;
    void *              start_routine_arg;
    void *              ret_value;

    volatile int        joined;
    volatile int        exited;
    volatile int        detached;

    void *              stack;
} mythread_t;

int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg);

int mythread_join(mythread_t *th, void **ret_value);

int mythread_equals(mythread_t *th1, mythread_t *th2);

#endif
