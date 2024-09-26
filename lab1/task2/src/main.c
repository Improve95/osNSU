#include "head.h"
#include "mythread.h"

void *mythread_function(void *arg) {
    printf("Hello from mythread\n");

    int *number = malloc(sizeof(int));
    *number = 25;
    
    return number;
}

int main() {
    __mythread *tid;
    int err;
    
    err = mythread_create(&tid, mythread_function, NULL);
    if (err != 0) {
        perror("mythread_create");
        return err;
    }

    int *ret_value = NULL;
    mythread_join(tid, (void *) &ret_value);

    printf("ret value from thread: %d\n", *ret_value);

    free(ret_value);

    return 0;
}
