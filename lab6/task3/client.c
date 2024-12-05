#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    int socket_fd = open("socket_fd", O_RDWR | O_CREAT, 0660);
    if (socket_fd == -1) {
        perror("socket_fd");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}