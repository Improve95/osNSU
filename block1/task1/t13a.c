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

void *mythread2(void *args) {
    int size = 7 * 1024 * 1024 / 4;
    int arr[size];
    memset(arr, 0, size);
    sleep(10);
}

void *mythread(void *args) {
    pthread_t tid;
    test_struct *give_struct = (test_struct *) args;
    for (int i = 0; i < 10; ++i) {
        // printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
        printf("mythread [%d %s]\n", give_struct->number, give_struct->char_array);
        usleep(50000);

        if (i == 5) {
            int err = pthread_create(&tid, NULL, mythread2, NULL);
            if (err) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
        }
    }
    pthread_cancel(tid);
}

int main() {
    int err = 0;
    pthread_t tid;

    test_struct stack_struct;
    stack_struct.number = 10;
    stack_struct.char_array = malloc(30 * sizeof(char));
    snprintf(stack_struct.char_array, 30, "hello world from stack struct");

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid, NULL, mythread, (void *)&stack_struct);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_exit(0);

    return err;
}
