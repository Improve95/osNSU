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
    test_struct *test = (test_struct *) args;
    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    print("mythread [%d %s]", test->number, test->char_array);
    sleep(3);
}

int main() {
    int err;
    pthread_attr_t attr;
    pthread_t tid;

    test_struct *test = malloc(sizeof(test_struct));
    test->number = 10;
    test->char_array = malloc(30 * sizeof(char));
    snprintf(test->char_array, 30, "hello world from struct");

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid, &attr, mythread, (void *)test);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    return 0;
}
