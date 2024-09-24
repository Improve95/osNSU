#ifndef MY_THREAD
#define MY_THREAD

#include "head.h"

typedef void *(*routine_func) (void *);

typedef struct {
    void *(*start_routine) (void *);
    void *arg;
} give_parameters_t;

typedef struct {
    int pid;

} mythread_t;


int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg);

#endif
