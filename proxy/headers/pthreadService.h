#ifndef PTHREADSERVICE_H
#define PTHREADSERVICE_H

#include <stdbool.h>
#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"

int initMutex(pthread_mutex_t *mutex);

int initCondVariable(pthread_cond_t *cond);

#endif //PTHREADSERVICE_H
