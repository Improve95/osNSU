#include "head.h"
#include "mythread.h"

void *thread_function(void *arg) {
    printf("Hello from mythread\n");
}

int main() {
    __mythread tid;
    int err;
    
    err = mythread_create(&tid, thread_function, NULL);
    if (err != 0) {
        perror("mythread_create");
        return err;
    }

    return 0;
}