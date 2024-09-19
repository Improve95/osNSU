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
    while (1) {
        printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());
        sleep(1);
    }
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

    sleep(3);
    pthread_cancel(tid);

    pthread_join(tid, NULL);
    
    // sleep(2);
    // pthread_exit(NULL);

    return 0;
}
