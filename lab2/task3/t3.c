#include "head.h"
#include "queue.h"

#define LIST_SIZE 100

volatile int asc_count = 0;
volatile int asc_iter = 0;
volatile int desc_count = 0;
volatile int desc_iter = 0;
volatile int eq_count = 0;
volatile int eq_iter = 0;
volatile int swap_count = 0;
volatile int swap_iter = 0;

void *start_routine1(void *arg) {
    while (1) {
        sleep(1);
        printf("asc: %d/%d, desc: %d/%d, eq: %d/%d, swap: %d/%d\n", 
                asc_count, asc_iter, 
                desc_count, desc_iter,
                eq_count, eq_iter, 
                swap_count, swap_iter);
    }
}

void *start_routine2(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev_node = linked_list;
        Node *cur_node = NULL;

        pthread_mutex_lock(&prev_node->lock);
        while (prev_node->next != NULL) {
            cur_node = prev_node->next;

            long prev_len = strlen(prev_node->value);
            long cur_len = strlen(cur_node->value);
            if (prev_len < cur_len) {
                asc_count += 1;
            }

            pthread_mutex_unlock(&prev_node->lock);
            pthread_mutex_lock(&cur_node->lock);

            prev_node = cur_node;
        }
        asc_iter += 1;
        pthread_mutex_unlock(&prev_node->lock);
    }
}

void *start_routine3(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev_node = linked_list;
        Node *cur_node = NULL;

        pthread_mutex_lock(&prev_node->lock);
        while (prev_node->next != NULL) {
            cur_node = prev_node->next;

            long prev_len = strlen(prev_node->value);
            long cur_len = strlen(cur_node->value);
            if (prev_len > cur_len) {
                desc_count += 1;
            }

            pthread_mutex_unlock(&prev_node->lock);
            pthread_mutex_lock(&cur_node->lock);

            prev_node = cur_node;
        }
        desc_iter += 1;
        pthread_mutex_unlock(&prev_node->lock);
    }
}

void *start_routine4(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev_node = linked_list;
        Node *cur_node = NULL;

        pthread_mutex_lock(&prev_node->lock);
        while (prev_node->next != NULL) {
            cur_node = prev_node->next;

            long prev_len = strlen(prev_node->value);
            long cur_len = strlen(cur_node->value);
            if (prev_len == cur_len) {
                eq_count += 1;
            }

            pthread_mutex_unlock(&prev_node->lock);
            pthread_mutex_lock(&cur_node->lock);

            prev_node = cur_node;
        }
        eq_iter += 1;
        pthread_mutex_unlock(&prev_node->lock);
    }
}

void *start_routine5(void *arg) {
    Node *linked_list = (Node*) arg;


    while (1) {
        Node *prev = linked_list;
        Node *cur = NULL;

        pthread_mutex_lock(&prev->lock);
        while (prev->next != NULL) {
            cur = prev->next;
            if (rand() % 100 != 0) {
                pthread_mutex_unlock(&prev->lock);
                pthread_mutex_lock(&cur->lock);
                prev = cur;
                continue;
            }

            pthread_mutex_lock(&cur->lock);
            Node *cur_next_tmp = cur->next;
            if (cur_next_tmp == NULL) {
                pthread_mutex_unlock(&cur->lock);
                break;
            }

            pthread_mutex_lock(&cur_next_tmp->lock);
            prev->next = cur_next_tmp;
            pthread_mutex_unlock(&prev->lock);

            cur->next = cur_next_tmp->next;
            pthread_mutex_unlock(&cur->lock);

            cur_next_tmp->next = cur;

            swap_count++;
            prev = cur_next_tmp;
        }
        pthread_mutex_unlock(&prev->lock);
        swap_iter += 1;
    }
}

Node *generate_new_node() {
    int string_size = rand() % MAX_VALUE_SIZE;
    Node *node = malloc(sizeof(Node));
    memset(node->value, 1, string_size);
    pthread_mutex_init(&node->lock, NULL);
    return node;
}

int main() {
    int err = 0;

    pthread_t tids[7];

    Node* linked_list = NULL;

    srand(time(NULL));   
    for (int i = 0; i < LIST_SIZE; ++i) {
        Node *node = generate_new_node();
        add_node(&linked_list, node);
    }

    Node* tmp = linked_list;
    while (tmp->next != NULL) {
        printf("%ld ", strlen(tmp->value));
        tmp = tmp->next;
    }

    err = pthread_create(&tids[0], NULL, start_routine1, linked_list);
    if (err) {
        perror("pthread_create tid1");
        return -1;
    }

    err = pthread_create(&tids[1], NULL, start_routine2, linked_list);
    if (err) {
        perror("pthread_create tid2");
        return -1;
    }

    err = pthread_create(&tids[2], NULL, start_routine3, linked_list);
    if (err) {
        perror("pthread_create tid3");
        return -1;
    }

    err = pthread_create(&tids[3], NULL, start_routine4, linked_list);
    if (err) {
        perror("pthread_create tid4");
        return -1;
    }

    for (int i = 0; i < 3; i++) {
        err = pthread_create(&tids[4 + i], NULL, start_routine5, linked_list);
        if (err) {
            perror("pthread_create tid4-6");
            return -1;
        }
    }
       
    sleep(7);

    for (int i = 0; i < 7; ++i) {
        pthread_cancel(tids[i]);
    }

    for (int i = 0; i < 7; ++i) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}