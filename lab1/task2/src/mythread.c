#include "head.h"
#include "mythread.h"

static const int PAGE_SIZE = 4096;
static const int STACK_SIZE = 9 * PAGE_SIZE;

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
    __mythread *mythread = (__mythread *) arg;

    start_routine_t start_routine = mythread->start_routine;
    void *start_routine_arg = mythread->start_routine_arg;

    void *ret_value = start_routine(start_routine_arg);

    /* if (!mythread->detached) {
        while (!mythread->joined) {
            usleep(500000);
        }
    } */

    // mythread->ret_value = ret_value;
    mythread->finished = 1;

    // munmap(mythread->stack, STACK_SIZE);

    return 0;
}

int mythread_join(__mythread *th, void **ret_value) {
    th->joined = 1;
    while (!th->finished) {
        usleep(500000);
    }
    ret_value = &th->ret_value;
}

/* int mythread_equals(mythread_t thread1, mythread_t thread2) {
    return thread1 == thread2;
} */

int mythread_create(__mythread *newthread, void *(*start_routine) (void *), void *arg) {
    static mythread_t thread_number = 0;
    int err;
    
    void *stack = create_stack(STACK_SIZE);
    if (stack == NULL) {
        return -1;
    }

    mprotect(stack + PAGE_SIZE, STACK_SIZE - PAGE_SIZE, PROT_READ | PROT_WRITE);

    __mythread *mythread = (__mythread *)(stack + STACK_SIZE - sizeof(__mythread));
    mythread->mythread_id = thread_number;
    mythread->start_routine = (start_routine_t) start_routine;
    mythread->start_routine_arg = arg;
    mythread->ret_value = NULL;
    mythread->joined = 0;
    mythread->finished = 0;
    mythread->detached = 0;
    mythread->stack = stack;

    thread_number++;

    void *child_stack = (void *) mythread;

    err = clone(initial_function, mythread, CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | SIGCHLD, (void *) newthread);
    if (err != 0) {
        perror("clone");
        return err;
    }

    return err;
}
