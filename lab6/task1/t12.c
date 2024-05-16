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
    char val = 0;

    int fd = open("./shared_file", O_RDWR | O_CREAT, 0660);
    
    void *shared_map = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    char *buf = (char *)shared_map;
    
    int prev_res = 0;
    int run = 1;
    while (run) {
        for (int i = 0; i < SIZE; ++i) {
            unsigned char buf_res = buf[i];
            // printf("%d ", buf_res);
            // fflush(stdout);
            // sleep(1);

            printf("buf_res %d %d\n", buf_res, prev_res);
            if (buf_res < prev_res && buf_res != 0) {
                printf("sequence failure %c\n", buf_res);
                // run = 0;
                break;
            }
            prev_res = buf_res;
        }
    }

    munmap(shared_map, SIZE);
    return 0;
}
