#include "head.h"
#include "mythread.h"

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
    

    // void *ret_value = start_routine(arg);
    
    //сделать очистку памяти и всего всего всего
    return 0;
}

int mythread_create(mythread_t *newthread, void *(*start_routine) (void *), void *arg) {
    int stack_size = 7 * 1024 * 1024;
    int err;
    
    void *stack = create_stack(stack_size, 1);
    if (stack == NULL) {
        return -1;
    }

    give_parameters_t give_parameters;
    give_parameters.start_routine = start_routine;
    give_parameters.arg = arg;
    err = clone(initial_function, stack, CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PIDFD, &give_parameters);

    if (err != 0) {
        perror("clone");
        return err;
    }

    return err;
}

