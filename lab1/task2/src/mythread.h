#ifndef MY_THREAD
#define MY_THREAD

#include "head.h"

typedef void *(*start_routine_t) (void *);

typedef struct {
    int                 mythread_id;
    start_routine_t     start_routine;
    void *              start_routine_arg;
    void *              ret_value;
    int                 joined;
} mythread_t;

int mythread_create(mythread_t *, void *(*start_routine) (void *), void *arg);

#endif
