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

void *mythread(void *args) {
    // pthread_detach(pthread_self());
    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
    sleep(3);
}

int main() {
    int err;
    pthread_attr_t attr;
    pthread_t tid;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    err = pthread_create(&tid, &attr, mythread, NULL);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    sleep(1);

    return 0;
}
