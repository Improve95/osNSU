#ifndef MY_THREAD
#define MY_THREAD

#include "head.h"

typedef void *(*routine) (void *);

typedef struct {
    routine start_routine;
    void *start_routine_args;
} give_parameters_t;

typedef struct {
    int pid;

} mythread_t;


int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg);

#endif
