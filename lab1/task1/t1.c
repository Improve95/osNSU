#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int global = 5;
static int global_static = 10; 

void *mythread(void *arg) {
	int local = 15;
	static int local_static = 20;
	const int local_const = 25;

	printf("mythread [%d %d %d %ld]: Hello from mythread!\nVariables: global %p, global_static %p local %p local_static %p local_const %p\n", 
		getpid(), getppid(), gettid(), pthread_self(), &global, &global_static, &local, &local_static, &local_const);
	return NULL;
}

int main() {
	pthread_t tid[5];
	int err;

	int local = 15;
	static int local_static = 20;
	const int local_const = 25;

	printf("main [%d %d %d]: Hello from main!\nVariables: global %p, global_static %p local %p local_static %p local_const %p\n\n", 
		getpid(), getppid(), gettid(), &global, &global_static, &local, &local_static, &local_const);

	for (int i = 0; i < 5; i++) {
		err = pthread_create(&tid[i], NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
	}

	sleep(1);

	printf("\n");
	
	printf("ref threads is equal: %d\n", tid[0] == tid[1]);
	printf("thread is equal: %d\n", pthread_equal(tid[0], tid[1]));

	return 0;
}