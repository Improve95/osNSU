#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>

int global = 5;

void *mythread(void *arg) {
    int local = 10;
    printf("main [%d %d %d]: Hello from thread1!\nVariables: global %p, local %p\n\n", 
		getpid(), getppid(), gettid(), &global, &local);
    
    printf("global: %d\n", global);
    sleep(2);
    printf("global: %d\n", global);
}

void *mythread2(void *arg) {
    int local = 20;
    printf("main [%d %d %d]: Hello from thread2!\nVariables: global %p, local %p\n\n", 
		getpid(), getppid(), gettid(), &global, &local);
    
    sleep(1);
    global *= 10;
}

int main() {
	pthread_t tid[2];
	int err;

	int local = 15;

    err = pthread_create(&tid[0], NULL, mythread, NULL);
    err = pthread_create(&tid[0], NULL, mythread2, NULL);
    if (err) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

	printf("main [%d %d %d]: Hello from main!\nVariables: global %p, local %p\n\n", 
		getpid(), getppid(), gettid(), &global, &local);

	for (int i = 0; i < 60; ++i) {
        sleep(1);
    }

	printf("\n");
	
	printf("ref threads is equal: %d\n", tid[0] == tid[1]);
	printf("thread is equal: %d\n", pthread_equal(tid[0], tid[1]));

	return 0;
}