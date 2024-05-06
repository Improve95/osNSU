#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#define HEAP_CAP 25 * 4096
#define HEAP_ALLOCED_CAP 1024
#define HEAP_FREED_CAP HEAP_ALLOCED_CAP

static void *heap;
static size_t heap_size = 0;

typedef struct {
    void *start;
    size_t size;
} Heap_Chunk;

Heap_Chunk heap_alloced[HEAP_ALLOCED_CAP] = {0};
static size_t heap_alloced_size = 0;

Heap_Chunk heap_freed[HEAP_FREED_CAP];

void * my_malloc(size_t size) {
    if (size > 0) {
        assert(heap_size + size <= HEAP_CAP);
        void *result = heap + heap_size;
        heap_size += size;

        const Heap_Chunk chunk = {
            .start = result,
            .size = size,
        };

        assert(heap_alloced_size <= HEAP_ALLOCED_CAP);

        heap_alloced[heap_alloced_size] = chunk;
        heap_alloced_size++;

        return result;  
    } else {
        return 0;
    }
}

void heap_dump_alooced_chunks() {
    printf("Alloced chunks %ld:\n", heap_alloced_size);
    for (size_t i = 0; i < heap_alloced_size; ++i) {
        printf("start: %p, size: %ld\n", heap_alloced[i].start, heap_alloced[i].size);
    }
}

void my_free(void *ptr) {
    for (size_t i = 0; i < heap_alloced_size; ++i) {
        if (heap_alloced[i].start == ptr) {
            
        }
    }
}


int main() {
    heap = mmap(NULL, HEAP_CAP, PROT_NONE | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if ((size_t)heap == -1) {
        perror("maps");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {
        void *a = (int *)my_malloc(5);
    }

    heap_dump_alooced_chunks();

    // my_free(a);

    munmap(heap, HEAP_CAP);
    return 0;
}
