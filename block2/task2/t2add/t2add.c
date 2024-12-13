#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>

#include "queue.h"

#define RED "\033[41m"
#define NOCOLOR "\033[0m"

typedef struct _test {
	pthread_mutex_t lock;
} test;

void set_cpu(int n) {
	int err;
	cpu_set_t cpuset;
	pthread_t tid = pthread_self();

	CPU_ZERO(&cpuset);
	CPU_SET(n, &cpuset);

	err = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpuset);
	if (err) {
		printf("set_cpu: pthread_setaffinity failed for cpu %d\n", n);
		return;
	}

	printf("set_cpu: set cpu %d\n", n);
}

void *start_routine1(void *arg) {
	int err = 0;
	test *test_struct = (test *) arg;
	err = pthread_mutex_lock(&test_struct->lock);
	if (err) {
		perror("pthread_mutex_lock t1");
		return NULL;
	}
	printf("mutex lock in t1\n");

	sleep(5);

	err = pthread_mutex_unlock(&test_struct->lock);
	if (err) {
		perror("pthread_mutex_unlock t1");
		return NULL;
	}
	printf("mutex unlock in t1\n");

	return NULL;
}

void *start_routine2(void *arg) {
	int err = 0;
	test *test_struct = (test *) arg;
	
	sleep(2);

	printf("try mutex unlock in t2\n");
	err = pthread_mutex_unlock(&test_struct->lock);
	printf("err: %d\n", err);
	if (err) {
		perror("pthread_mutex_unlock t2");
		return NULL;
	}
	printf("mutex unlock in t2\n");

	return NULL;
}

int main() {
	pthread_t tid1;
	pthread_t tid2;
	int err;

	printf("main [%d %d %d]\n", getpid(), getppid(), gettid());

	test *test_struct = malloc(sizeof(test));

	err = pthread_create(&tid1, NULL, start_routine1, test_struct);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	err = pthread_create(&tid2, NULL, start_routine2, test_struct);
	if (err) {
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	return 0;
}
