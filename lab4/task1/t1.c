#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

int uninitialized_global;
int initialized_global = 5;
const int const_global = 5;

void f1() {
    int local = 5;
    const int const_local = 5;
    static int static_local = 5;

    int *arr = malloc(1000 * sizeof(int));

    printf("uninitialized_global: %p\n", &uninitialized_global);
    printf("initialized_global: %p\n", &initialized_global);
    printf("const_global: %p\n", &const_global);
    printf("local: %p\n", &local);
    printf("const_local: %p\n", &const_local);
    printf("static_local: %p\n", &static_local);
    printf("arr: %p\n", arr);
    printf("arr: %p\n", &arr);
    printf("pid: %d\n", getpid());

    while(1) {
        sleep(10);
    }
}

void f2() {
    char *arr1 = malloc(100);
    strcpy(arr1, "Hello, World!");
    printf("print1: %s\n", arr1);

    free(arr1);
    printf("print2: %s\n", arr1);


    char *arr2 = malloc(100);
    strcpy(arr2, "Hello, World!");
    printf("print3: %s\n", arr2);

    arr2 += 50;
    free(arr2);
    printf("print: %s\n", arr2);
}

int * f4i() {
    int a = 60;
    int *b = &a;
    printf("%p\n", b);
    return b;
}

void f4() {
    int *a = f4i();
    printf("%p\n", a);
    printf("%d\n", *a);
}

void f3() {
    char *env = getenv("MY_VAR");
    env = "fsdfasd";
    printf("%s\n", env);
}

int main() {
    // f1();
    // f2();
    // f3();
    f4();

    return 0;
}
