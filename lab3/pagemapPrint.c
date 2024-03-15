#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 256
#define PAGE_SIZE 4096

void read_pagemap(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    unsigned long long pagemap_entry;
    unsigned long long pfn;
    int page_present;
    int page_swapped;
    int page_file_mapped;
    int page_file_offset;

    while (fread(&pagemap_entry, sizeof(unsigned long long), 1, file) == 1) {
        pfn = pagemap_entry & ((1ULL << 55) - 1); // Берем только младшие 55 бит (биты 0-54)
        page_present = (pagemap_entry >> 63) & 1; // Берем бит 63, который указывает на присутствие страницы в памяти
        page_swapped = (pagemap_entry >> 62) & 1; // Берем бит 62, который указывает на обмен страницы на диск
        page_file_mapped = (pagemap_entry >> 61) & 1; // Берем бит 61, который указывает на сопоставление страницы с файлом
        page_file_offset = (pagemap_entry >> 0) & ((1ULL << 55) - 1); // Берем биты 0-54, которые указывают смещение в файле (если страница сопоставлена с файлом)

        printf("PFN: 0x%llx, Present: %d, Swapped: %d, File Mapped: %d, File Offset: %d\n", 
                pfn, page_present, page_swapped, page_file_mapped, page_file_offset);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char path[MAX_PATH_LENGTH];
    snprintf(path, MAX_PATH_LENGTH, "/proc/%s/pagemap", argv[1]);

    read_pagemap(path);

    return 0;
}
