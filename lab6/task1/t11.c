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

#define SIZE 4096

int main() {
    unsigned char val = 0;

    int fd = open("./shared_file", O_RDWR | O_CREAT, 0660);
    if (fd == -1) {
        perror("open shared_file");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(fd, 0) == -1 || ftruncate(fd, SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }
    
    void *shared_map = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    char *buf = (char *)shared_map;

    while (1) {
        for (int i = 0; i < SIZE; ++i) {
            buf[i] = val++;
            // sleep(1);
        }
    }

    munmap(shared_map, SIZE);
    return 0;
}