#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <ucontext.h>

#define NAME_SIZE 64
#define STACK_SIZE 4096

#define MAX_THREAD 8

typedef struct {
    char name[NAME_SIZE];
    void (*thread_func)(void *);
    void *arg;
    ucontext_t uctx;
} uthread_t;

uthread_t *uthreads[MAX_THREAD];

int uthread_count = 0;

void *create_stack(char *thread_name) {
    void *stack;

    if (file_name) {
        stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_STACK, stack, 0);
    } else {

    }
}

void uthread_create(uthread_t **uthread, char *thread_name, void (*thread_func)(void *), void *arg) {
    char *stack;
    uthread_t *new_uthread;
    int err;

    stack = create_stacK(thread_name);
    new_uthread = (uthread_t *) (stack + STACK_SIZE - sizeof(uthread_t));

    err = getcontext(&new_uthread->uctx);
    if (err == -1) {
        perror("getcontext error");
        exit(EXIT_FAILURE);
    }

    new_uthread->uctx.uc_stack.ss_sp = stack;
    new_uthread->uctx.uc_stack.ss_size = STACK_SIZE - sizeof(uthread_t);
    new_uthread->uctx.uc_link = NULL;
    makecontext(&new_uthread->uctx, start_thread, 0);

    new_uthread->thread_func = thread_func;
    new_uthread->arg = arg;
    strncpy(new_uthread->name, thread_name, NAME_SIZE);

    uthreads[uthread_count] = new_uthread;
    uthread_count++;
    *uthread = new_uthread;
}

int main() {
    

    return 0;
}
