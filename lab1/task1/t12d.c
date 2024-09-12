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
    printf("mythread [%d %d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid(), 0);
}

int main() {
    int err;
    const int thread_number = 100000;
    pthread_t tid[thread_number];

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    for (int i = 0; i < thread_number; i++) {
        err = pthread_create(&tid[i], NULL, mythread, NULL);
        if (err) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        // sleep(1);
    }

    return 0;
}
