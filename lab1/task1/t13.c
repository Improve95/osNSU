#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    int number;
    char *char_array;
} test_struct;

void *mythread(void *args) {
    test_struct *give_struct = (test_struct *) args;
    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    printf("mythread [%d %s]\n", give_struct->number, give_struct->char_array);
    sleep(3);
}

int main() {
    int err;
    pthread_t tid;

    test_struct stack_struct;
    stack_struct.number = 10;
    stack_struct.char_array = malloc(30 * sizeof(char));
    snprintf(stack_struct.char_array, 30, "hello world from stack struct");

    test_struct *heap_struct = malloc(sizeof(test_struct));
    heap_struct->number = 20;
    heap_struct->char_array = malloc(30 * sizeof(char));
    snprintf(heap_struct->char_array, 30, "hello world from heap struct");

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid, NULL, mythread, (void *)heap_struct);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    return 0;
}
