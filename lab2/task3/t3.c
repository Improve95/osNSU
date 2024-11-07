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
                eq_count, eq_iter, swap_count, swap_iter);
    }
}

void *start_routine2(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev = linked_list;
        Node *cur = NULL;

        pthread_mutex_lock(&prev->lock);
        while (prev->next != NULL) {
            cur = prev->next;

            long prev_len = strlen(prev->value);
            long cur_len = strlen(cur->value);
            if (prev_len < cur_len) {
                asc_count += 1;
            }

            pthread_mutex_unlock(&prev->lock);
            pthread_mutex_lock(&cur->lock);

            prev = cur;
        }
        asc_iter += 1;
        pthread_mutex_unlock(&cur->lock);
    }
}

void *start_routine3(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev = linked_list;
        Node *cur = NULL;

        pthread_mutex_lock(&prev->lock);
        while (prev->next != NULL) {
            cur = prev->next;

            long prev_len = strlen(prev->value);
            long cur_len = strlen(cur->value);
            if (prev_len > cur_len) {
                desc_count += 1;
            }

            pthread_mutex_unlock(&prev->lock);
            pthread_mutex_lock(&cur->lock);

            prev = cur;
        }
        desc_iter += 1;
        pthread_mutex_unlock(&cur->lock);
    }
}

void *start_routine4(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
		pthread_testcancel();

        Node *prev = linked_list;
        Node *cur = NULL;

        pthread_mutex_lock(&prev->lock);
        while (prev->next != NULL) {
            cur = prev->next;

            long prev_len = strlen(prev->value);
            long cur_len = strlen(cur->value);
            if (prev_len == cur_len) {
                eq_count += 1;
            }

            pthread_mutex_unlock(&prev->lock);
            pthread_mutex_lock(&cur->lock);

            prev = cur;
        }
        eq_iter += 1;
        pthread_mutex_unlock(&cur->lock);
    }
}

Node *find_by_index(Node* linked_list, int index) {
    Node *prev = linked_list;
    Node *cur = linked_list;

    for (int i = 0; i < index && prev->next != NULL; i++) {
        cur = prev->next;
        prev = cur;
    }

    return cur;
}

void *start_routine5(void *arg) {
    Node* linked_list = (Node*) arg;

    while (1) {
        pthread_testcancel();

        Node *prev = linked_list;
        Node *cur = NULL;

        pthread_mutex_lock(&prev->lock);
        while (prev->next != NULL) {
            cur = prev->next;

            int random_place_1 = rand() % LIST_SIZE;
            if (random_place_1 == 0) {
                pthread_mutex_unlock(&prev->lock);
                pthread_mutex_lock(&cur->lock);
                prev = cur;
                continue;
            }

            Node* replaceNode = find_by_index(linked_list, random_place_1 - 1);
            
            

            pthread_mutex_unlock(&prev->lock);
            pthread_mutex_lock(&cur->lock);

            prev = cur;
        }
        pthread_mutex_unlock(&cur->lock);
    }
}

Node *generate_new_node() {
    int string_size = rand() % MAX_VALUE_SIZE;
    Node *node = malloc(sizeof(Node));
    memset(node->value, 1, string_size);
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