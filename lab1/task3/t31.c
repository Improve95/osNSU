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
#include <string.h>

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

static volatile int uthread_count = 0;
static volatile int current_thread = 0;

void *create_stack(int file_size, char *file_name) {
    int stack_fd = open(file_name, O_RDWR | O_CREAT, 0660);
    ftruncate(stack_fd, 0);
    ftruncate(stack_fd, file_size);

    void *stack = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_STACK, stack_fd, 0);
    close(stack_fd);
    
    return stack;
}

void schedule() {

    int err;
    ucontext_t *cur_ctx, *next_ctx;

    cur_ctx = &(uthreads[current_thread]->uctx);

    current_thread = (current_thread + 1) % uthread_count;
    next_ctx = &(uthreads[current_thread]->uctx);

    err = swapcontext(cur_ctx, next_ctx);
    if (err == -1) {
        perror("swap context");
        exit(1);
    }
}

void start_thread() {
    for (int i = 0; i < uthread_count; i++) {
        ucontext_t *ctx = &uthreads[i]->uctx;
        char *stack_from = ctx->uc_stack.ss_sp;
        char *stack_to = ctx->uc_stack.ss_sp + ctx->uc_stack.ss_size;
        if (stack_from <= (char*)&i && (char*)&i <= stack_to) {
            printf("start thread: i=%d name: %s thread_func: %p, arg: %p", 
                i, uthreads[i]->name, uthreads[i]->thread_func, uthreads[i]->arg);

                uthreads[i]->thread_func(uthreads[i]->arg);
        } 
    }
}

void uthread_create(uthread_t **uthread, char *thread_name, void (*thread_func)(void *), void *arg) {
    char *stack;
    uthread_t *new_uthread;
    int err;

    stack = create_stack(STACK_SIZE, thread_name);
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

void mythread1(void *arg) {
    int i;
    char *myarg = (char *) arg;

    printf("mythread1 started, arg: %s, %p %d %d %d\n", myarg, &i, getpid(), getppid(), gettid());

    for (i = 0; i < 10; i++) {
        printf("mythread1, arg %s %p schedule()\n", myarg, &i);
        sleep(1);
        schedule();
    }

    printf("mythread1: finished\n");
}

void mythread2(void *arg) {
    int i;
    char *myarg = (char *) arg;

    printf("mythread2 started, arg: %s, %p %d %d %d\n", myarg, &i, getpid(), getppid(), gettid());

    for (i = 0; i < 10; i++) {
        printf("mythread2, arg %s %p schedule()\n", myarg, &i);
        sleep(1);
        schedule();
    }

    printf("mythread2: finished\n");
}

int main() {
    uthread_t *ut[3];
    char *arg[] = {"arg000", "arg111", "arg222"};

    uthread_t main_thread;
    uthreads[0] = &main_thread;
    uthread_count = 1;

    printf("main: started: %d %d %d\n", getpid(), getppid(), gettid());

    uthread_create(&ut[0], "user-thread-0", mythread1, (void *)arg[0]);
    uthread_create(&ut[1], "user-thread-1", mythread2, (void *)arg[1]);
    uthread_create(&ut[2], "user-thread-2", mythread1, (void *)arg[2]);

    while (1) {
        printf("main thread\n");
        schedule();
    }

    printf("main: finished: %d %d %d\n", getpid(), getppid(), gettid());

    return 0;
}
