#define _GNU_SOURCE
#include <pthread.h>
#include <assert.h>

#include "queue.h"

void *qmonitor(void *arg) {
	queue_t *q = (queue_t *)arg;

	printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

	while (1) {
		queue_print_stats(q);
		sleep(1);
	}

	return NULL;
}

queue_t* queue_init(int max_count) {
	int err;

	queue_t *q = malloc(sizeof(queue_t));
	if (!q) {
		printf("Cannot allocate memory for a queue\n");
		abort();
	}

	q->first = NULL;
	q->last = NULL;
	q->max_count = max_count;
	q->count = 0;

	q->add_attempts = q->get_attempts = 0;
	q->add_count = q->get_count = 0;

	err = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
	if (err) {
		printf("queue_init: pthread_create() failed: %s\n", strerror(err));
		abort();
	}

	return q;
}

void queue_destroy(queue_t *q) {
	printf("\n== destroy ==\n");

	pthread_cancel(q->qmonitor_tid);
	qnode_t *current_node = q->first;
	while (current_node != NULL) {
		qnode_t *tmp = current_node->next;
		free(current_node);
		current_node = tmp;
	}	
	free(q);

	pthread_join(q->qmonitor_tid, NULL);
	// TODO: It's needed to implement this function
}

int queue_add(queue_t *q, int val) {
	q->add_attempts++;

	assert(q->count <= q->max_count);

	if (q->count == q->max_count)
		return 0;

	qnode_t *newQNode = malloc(sizeof(qnode_t));
	if (!newQNode) {
		printf("Cannot allocate memory for newQNode node\n");
		abort();
	}

	newQNode->val = val;
	newQNode->next = NULL;

	if (!q->first)
		q->first = q->last = newQNode;
	else {
		q->last->next = newQNode;
		q->last = q->last->next;
	}

	q->count++;
	q->add_count++;

	return 1;
}

int queue_get(queue_t *q, int *val) {
	q->get_attempts++;

	assert(q->count >= 0);

	if (q->count == 0)
		return 0;

	qnode_t *tmp = q->first;

	*val = tmp->val;
	q->first = q->first->next;

	free(tmp);
	q->count--;
	q->get_count++;

	return 1;
}

void queue_print_stats(queue_t *q) {
	printf("queue stats: current size %d; attempts: (get: %ld, add: %ld, %ld); counts (get: %ld, add: %ld, %ld)\n",
		q->count,
		q->add_attempts, q->get_attempts, q->add_attempts - q->get_attempts,
		q->add_count, q->get_count, q->add_count -q->get_count);
}

