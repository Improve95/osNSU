#include "head.h"
#include "mypthread.h"

void *create_stack(int stack_size, int thread_number) {
    void *stack;

    int stack_fd = open("./stack", O_RDWR | O_CREAT, 0660);

    stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
}

int mpthread_create() {
    int err;
    
    void *stack = create_stack();
    if (stack == NULL) {

    }

    err = clone();
    if (err) {
        perror("clone");
        return err;
    }

    return err;
}

