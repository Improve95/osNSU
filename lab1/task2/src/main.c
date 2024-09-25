#include "head.h"
#include "mythread.h"

void *mythread_function(void *arg) {
    printf("Hello from mythread\n");
    return "ret value from mythread";
}

int main() {
    __mythread *tid;
    int err;
    
    err = mythread_create(&tid, mythread_function, NULL);
    if (err != 0) {
        perror("mythread_create");
        return err;
    }

    char *ret_value = malloc(30);
    mythread_join(tid, (void *) &ret_value);
    printf("ret value from thread: %s\n", ret_value);

    free(ret_value);

    return 0;
}
