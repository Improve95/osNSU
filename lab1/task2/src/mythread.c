#include "head.h"
#include "mythread.h"

static const int STACK_SIZE = 7 * 1024 * 1024;

void *create_stack(int stack_size) {
    void *stack;

    int stack_fd = open("./stack", O_RDWR | O_CREAT, 0660);
    if (stack_fd == -1) {
        perror("open stack");
        return NULL;
    }
    ftruncate(stack_fd, 0);
    ftruncate(stack_fd, stack_size);

    stack = mmap(NULL, stack_size, PROT_NONE, MAP_SHARED, stack_fd, 0);
    if (stack == NULL) {
        perror("mmap");
        return NULL;
    }

    close(stack_fd);

    return stack;
}

int initial_function(void *arg) {
    mythread_t *mythread = (mythread_t *) arg;

    start_routine_t start_routine = mythread->start_routine;
    void *start_routine_arg = mythread->start_routine_arg;

    void *ret_value = start_routine(start_routine_arg);

    if (!mythread->detached) {
        while (mythread->joined) {
            usleep(500000);
        }
    }

    mythread->ret_value = ret_value;
    mythread->exited = 1;

    // mythread.

    return 0;
}

int mythread_join(mythread_t *th, void **ret_value) {
    th->joined = 1;
    while (!th->exited) {
        usleep(500000);
    }
    ret_value = &th->ret_value;
}

int mythread_equals(mythread_t *th1, mythread_t *th2) {
    if (th1->mythread_id == th2->mythread_id &&
        th1->start_routine == th2->start_routine &&
        &(th1->start_routine_arg) == &(th2->start_routine_arg) &&
        th1->ret_value == th2->ret_value &&
        th1->joined == th2->joined &&
        th1->exited == th2->exited &&
        th1->detached == th2->detached) {
        return 1;
    }
    return 0;
}

int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg) {
    static int thread_number = 0;
    int err;
    
    void *stack = create_stack(STACK_SIZE);
    if (stack == NULL) {
        return -1;
    }

    mprotect(stack, STACK_SIZE, PROT_READ | PROT_WRITE);

    newthread->mythread_id = ++thread_number;
    newthread->start_routine = (start_routine_t) start_routine;
    newthread->start_routine_arg = arg;
    newthread->ret_value = NULL;
    newthread->joined = 0;
    newthread->exited = 0;
    newthread->detached = 0;
    newthread->stack = stack;

    err = clone(initial_function, stack, CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | SIGCHLD, (void *) newthread);

    if (err != 0) {
        perror("clone");
        return err;
    }

    return err;
}

