#include "head.h"
#include "mythread.h"

static const int STACK_SIZE = 7 * 1024 * 1024;

void *create_stack(int stack_size, int thread_number) {
    void *stack;

    int stack_fd = open("./stack", O_RDWR | O_CREAT, 0660);
    if (stack_fd == -1) {
        perror("open stack");
        return NULL;
    }
    ftruncate(stack_fd, 0);
    ftruncate(stack_fd, stack_size);

    stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
    if (stack == NULL) {
        perror("mmap");
        return NULL;
    }

    return stack;
}

int initial_function(void *arg) {
    mythread_t *mythread = (mythread_t *) arg;

    start_routine_t start_routine = mythread->start_routine;
    void *start_routine_arg = mythread->start_routine_arg;

    void *ret_value = start_routine(start_routine_arg);

    //wait
    //joined

    //сделать завершение работы потока
    return 0;
}

int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg) {
    static int thread_number = 0;
    int err;
    
    void *stack = create_stack(STACK_SIZE, ++thread_number);
    if (stack == NULL) {
        return -1;
    }

    newthread->start_routine = (start_routine_t) start_routine;
    newthread->start_routine_arg = arg;

    err = clone(initial_function, stack, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PIDFD, (void *) newthread);

    if (err != 0) {
        perror("clone");
        return err;
    }

    return err;
}

