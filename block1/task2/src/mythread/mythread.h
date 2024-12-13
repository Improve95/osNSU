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

/* creates a unique and inimitable, hand-made stream, starting with execution of START-ROUTINE
   getting passed ARG.
   ret-value:    0 - ok;
                -1 - stack allocate error
                -2 - mprotect allocate error
                -3 - create thread error */
int mythread_create(__mythread **tid, void *(*start_routine) (void *), void *arg);

int mythread_join(__mythread *th, void **ret_value);

int mythread_equals(__mythread *thread1, __mythread *thread2);

#endif
