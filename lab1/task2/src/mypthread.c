#include "head.h"
#include "mypthread.h"

int mpthread_create() {
    int err;

    

    err = clone();
    if (err) {
        perror("clone");
        return err;
    }

    return err;
}

