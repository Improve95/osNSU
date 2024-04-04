#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

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
    sleep(7);
    int s = 100;
    char *a[s];
    for (int i = 0; i < s; i++) {
        a[i] = malloc(1000);
        // usleep(100000);
        fillSize += 1000;
        printf("%ld\n", fillSize);
    }
    for (int i = 0; i < s; i++) {
        free(a[i]);
    }
}

void f4() {
    printf("mmap initialize\n");

    void *region = mmap(NULL, 10 * 4096, PROT_READ | PROT_WRITE | PROT_EXEC,  MAP_PRIVATE | (32), -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("region: %p\n", region);

    sleep(10);

    mprotect(region, 10 * 4096, PROT_NONE);

    sleep(10);
}

int main(/*int argc, char *argv[]*/) {
    // f1(argc, argv);
    // f2();


    printf("pid: %d\n", getpid());
    // f3();

    return 0;
}

#pragma GCC diagnostic pop
