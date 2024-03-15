#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>

#define PAGEMAP_ENTRY 8
#define GET_BIT(X,Y) (X & ((uint64_t)1<<Y)) >> Y
#define GET_PFN(X) X & 0x7FFFFFFFFFFFFF

char path_buf [0x100] = {};

int read_pagemap(char * path_buf, unsigned long virt_addr) {
    FILE *fileIn = fopen(path_buf, "rb");
    if (!fileIn) {
        printf("Error! Cannot open %s\n", path_buf);
        return -1;
    }

    //Shifting by virt-addr-offset number of bytes
    //and multiplying by the size of an address (the size of an entry in pagemap file)
    uint64_t file_offset = virt_addr / getpagesize() * PAGEMAP_ENTRY;
    printf("Vaddr: 0x%lx, Page_size: %d, Entry_size: %d\n", virt_addr, getpagesize(), PAGEMAP_ENTRY);
    printf("Reading %s at 0x%llx\n", path_buf, (unsigned long long) file_offset);

    if (fseek(fileIn, file_offset, SEEK_SET)) {
        perror("Failed to do fseek!");
        return -1;
    }

    errno = 0;
    uint64_t read_val = 0;
    unsigned char c_buf[PAGEMAP_ENTRY];
    for(size_t i = 0; i < PAGEMAP_ENTRY; i++) {
        char c = getc(fileIn);
        if(c == EOF){
            printf("\nReached end of the file\n");
            return 0;
        }
        c_buf[i] = c;
    }
    for (size_t i = 0; i < PAGEMAP_ENTRY; i++) {
        //printf("%d ",c_buf[i]);       
        read_val = (read_val << 8) + c_buf[i];
    }
    printf("\n");
    printf("Result: 0x%llx\n", (unsigned long long) read_val);
    //if(GET_BIT(read_val, 63))
    if(GET_BIT(read_val, 63)) {
        printf("PFN: 0x%llx\n",(unsigned long long) GET_PFN(read_val));
    } else {
        printf("Page not present\n");
    }
    if(GET_BIT(read_val, 62)) {
        printf("Page swapped\n");
    }

    fclose(fileIn);
    return 0;
}

int main(int argc, char *argv[]) {
    //printf("%lu\n", GET_BIT(0xA680000000000000, 63));
    //return 0;
    if (argc != 3) {
        printf("Argument number is not correct!\n pagemap PID VIRTUAL_ADDRESS\n");
        return -1;
    }

    int pid = 0;
    if (!memcmp(argv[1], "self", sizeof("self"))) {
        sprintf(path_buf, "/proc/self/pagemap");
        pid = -1;
    } else {
        char *end;
        pid = strtol(argv[1],&end, 10);
        if (end == argv[1] || *end != '\0' || pid<=0) {
            printf("PID must be a positive number or 'self'\n");
            return -1;
        }
    }

    size_t virt_addr = strtol(argv[2], NULL, 16);
    if(pid != -1) {
        sprintf(path_buf, "/proc/%u/pagemap", pid);
    }

    read_pagemap(path_buf, virt_addr);
    return 0;
}
