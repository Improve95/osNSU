#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int recursion(int recursion_depth) {
    if (recursion_depth > 0) {
        char str[10];
        snprintf(str, 10, "hello-%d", recursion_depth);
        recursion_depth--;
        recursion(recursion_depth);
    } else {
        printf("end of recursion");
    }
}

int child_func() {
    int recursion_depth = 10;
    recursion(recursion_depth);
    
    printf("\nin child process: pid %d, ppid %d\n", getpid(), getppid());
    return 0;
}

int main() {
    int fd = open("/home/opp112/os/osNSU/lab5/task3/stack", O_RDWR);
    if (fd == -1) {
        perror("file open");
        exit(EXIT_FAILURE);
    }

    int stack_size = 1024 * 1024;
    if (ftruncate(fd, stack_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    void *stack = mmap((caddr_t)0, stack_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int clone_pid = clone(child_func, stack + stack_size, CLONE_FS, NULL);

    if (clone_pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    printf("in parent process pid %d, child pid %d\n", getpid(), clone_pid);

    waitpid(clone_pid, NULL, 0);
    munmap(stack, stack_size);
    return 0;
}