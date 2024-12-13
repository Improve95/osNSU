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

    printf("mythread [%d %d %d]: Hello from mythread!\n", getpid(), getppid(), gettid());

    sleep(3);

    printf("mythread [%d %d %d]: goodbye from mythread!\n", getpid(), getppid(), gettid());

    char *ret_value = malloc(30 * sizeof(char));
    printf("mythread: %p\n", ret_value);

    snprintf(ret_value, 30, "hello world from thread");
    return ret_value;
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

    char *mythread_ret = NULL;
    pthread_join(tid, (void *) *mythread_ret);
    printf("main: %p\n", mythread_ret);

    printf("main [%d %d %d]: mythread is over\n", getpid(), getppid(), gettid());
    printf("return value: %s\n", mythread_ret);

    free(mythread_ret);

    return 0;
}
