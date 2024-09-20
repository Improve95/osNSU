#ifndef MY_PTHREAD
#define MY_PTHREAD

#include "head.h"

// Объявить тип функции

typedef struct {
    int pid;

} __mypthread;

void *create_stack();

void startup() {

}

int mpthread_create();

#endif
