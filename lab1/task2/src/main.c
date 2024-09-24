#include "head.h"
#include "mythread.h"

void *thread_function(void *args) {
    printf("Hello from mythread\n");
}

int main() {
    mythread_t tid;
    int err;
    
    err = mythread_create(&tid, thread_function, NULL);
    if (err != 0) {
        perror("mythread_create");
        return err;
    }

    return 0;
}