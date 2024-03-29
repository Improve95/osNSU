#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int uninitialized_global;
int initialized_global = 5;
const int const_global = 5;

void f1() {
    int local = 5;
    const int const_local = 5;
    static int static_local = 5;

    printf("uninitialized_global: %p\n", &uninitialized_global);
    printf("initialized_global: %p\n", &initialized_global);
    printf("const_global: %p\n", &const_global);
    printf("local: %p\n", &local);
    printf("const_local: %p\n", &const_local);
    printf("static_local: %p\n", &static_local);
    printf("pid: %d\n", getpid());

    while(1) {
        sleep(10);
    }
}

int main() {
    f1();

    return 0;
}