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

void cleanup(void *arg) {
    free(arg);
    printf("hello from cleanup\n");
}

void *mythread(void *args) {
    char *str = malloc(30);
    snprintf(str, 30, "hello world from thread\n");
    
    pthread_cleanup_push(cleanup, str);

    while (1) {
        printf("%s", str);
        sleep(1);
    }

    pthread_cleanup_pop(0);
}

int main() {
    int err;
    pthread_t tid;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid, NULL, mythread, NULL);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(2);
    pthread_cancel(tid);

    pthread_join(tid, NULL);
    return 0;
}
