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

#define SIZE 128

int main() {
    int fd_list[2];

    int err = pipe(fd_list);
    if (err == - 1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int fork_pid = fork();
    if (fork_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (fork_pid == 0) {
        char val;
        while (1) {
            for (int i = 0; i < SIZE * 2; ++i) {
                read(fd_list[0], &val, sizeof(val));
                printf("val: %c\n", val);
            }
        }
    } else {
        while (1) {
            for (int i = 0; i < SIZE * 2; ++i) {
                write(fd_list[1], &i, sizeof(char));
            }
        }
    }
    
    return 0;
}