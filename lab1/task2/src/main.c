#include "head.h"
#include "mythread.h"

void *mythread_function(void *arg) {
    printf("Hello from mythread\n");
}

int main() {
    __mythread *tid;
    int err;
    
    err = mythread_create(&tid, mythread_function, NULL);
    if (err != 0) {
        perror("mythread_create");
        return err;
    }

    mythread_join(tid, NULL);

    return 0;
}
