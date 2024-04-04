#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
void f1(int argc, char *argv[]) {
    printf("pid: %d\n", getpid());
    sleep(1);

    int status = execv("./t2.out", argv);
    printf("3: %p\n", "Hello world");

    if (status != 0 || argc != 0) {
        printf("not null return\n");
    }
}

size_t fillSize = 0;

void fillStack() {
    char a[4096];
    fillSize += 4096;
    printf("%ld\n", fillSize);
    usleep(500000);
    fillStack();
}

void f2() {
    printf("pid: %d\n", getpid());
    fillStack();
}

void f3() {
    printf("pid: %d\n", getpid());
    sleep(4);
    for (int i = 0; i < 100000; i++) {
        malloc(1000);
        usleep(100000);
        fillSize += 1000;
        printf("%ld\n", fillSize);
    }
}

void f4() {
    
}

int main(int argc, char *argv[]) {
    // f1(argc, argv);
    // f2();
    // f3();
    

    return 0;
}