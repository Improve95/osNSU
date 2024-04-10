#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>

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
    usleep(200000);
    fillStack();
}

void f2() {
    printf("pid: %d\n", getpid());
    fillStack();
}

void f3() {
    sleep(5);
    int s = 160;
    char *a[s];
    for (int i = 0; i < s; i++) {
        usleep(50000); 
        a[i] = malloc(1000);
        fillSize += 1000;
        printf("%ld\n", fillSize);
    }
    for (int i = 0; i < s; i++) {
        usleep(50000);
        free(a[i]);
        fillSize -= 1000;
        printf("%ld\n", fillSize);
    }
}

void handler(int signum) {
    printf("catch sigsegv\n");
    exit(1);
}

void f4() {
    sleep(5);

    printf("mmap initialize\n");

    void *region = mmap(NULL, 10 * 4096, PROT_NONE | PROT_READ | PROT_WRITE,  MAP_PRIVATE | (32), -1, 0);
    if (region == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    printf("region: %p\n", region);

    sleep(1);

    mprotect(region, 10 * 4096, PROT_NONE);

    sleep(5);

    // signal(SIGSEGV, handler);
    // int a = *((int*)region);

    munmap(region + 4 * 4096, 2 * 4096);

    sleep(5);
}

int main(int argc, char *argv[]) {
    // f1(argc, argv);

    // f2();

    printf("pid: %d\n", getpid());
    // f3();

    f4();

    return 0;
}
