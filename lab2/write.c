#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

int main() {
    int fd;
    char buffer[13] = "Hello world!";

    write(1, &buffer, 13);

    return 0;
}
