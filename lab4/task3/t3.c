#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <stdbool.h>

#define HEAP_CAP 25 * 4096
#define HEAP_ALLOCED_CAP 1024

static void *heap;
static size_t heap_size = 0;

typedef struct {
    void *start;
    size_t size;
    bool is_free;
} Heap_Chunk;

Heap_Chunk heap_alloced[HEAP_ALLOCED_CAP] = {0};
static size_t heap_alloced_size = 0;

void heap_dump_alooced_chunks() {
    printf("Alloced chunks %ld:\n", heap_alloced_size);
    for (size_t i = 0; i < heap_alloced_size; ++i) {
        printf("start: %p, size: %ld\n", heap_alloced[i].start, heap_alloced[i].size);
    }
}

void * replace_chunk(Heap_Chunk *chunk, size_t size) {
    chunk->size = size;
    chunk->is_free = false;
    printf("size: %ld\n", size);
    return chunk->start;
}

void * create_new_chunk(size_t size) {
    void *chunk_start = heap + heap_size;
        heap_size += size;

        const Heap_Chunk chunk = {
            .start = chunk_start,
            .size = size,
            .is_free = false,
        };

        assert(heap_alloced_size <= HEAP_ALLOCED_CAP);

        heap_alloced[heap_alloced_size] = chunk;
        heap_alloced_size++;

        return chunk_start;
}

void * my_malloc(size_t size) {
    if (size > 0) {
        assert(heap_size + size <= HEAP_CAP);
        
        for (size_t i = 0; i < heap_alloced_size; ++i) {
            if (heap_alloced[i].is_free == true && size <= heap_alloced[i].size) {
                replace_chunk(&heap_alloced[i], size);
                return heap_alloced[i].start;
            }
        }

        return create_new_chunk(size); 
    } else {
        return NULL;
    }
}

void my_free(void *ptr) {
    for (size_t i = 0; i < heap_alloced_size; ++i) {
        if (heap_alloced[i].start == ptr) {
            heap_alloced[i].is_free = true;
        }
    }
}

int main() {
    heap = mmap(NULL, HEAP_CAP, PROT_NONE | PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if ((size_t)heap == -1) {
        perror("maps");
        exit(EXIT_FAILURE);
    }

    void *(*ptr_a) = my_malloc(10 * sizeof(void *));
    for (int i = 0; i < 10; ++i) {
        ptr_a[i] = my_malloc(50);
    }

    heap_dump_alooced_chunks();

    my_free(ptr_a[2]);
    my_free(ptr_a[3]);
    my_free(ptr_a[4]);

    ptr_a[2] = my_malloc(40);

    heap_dump_alooced_chunks();

    munmap(heap, HEAP_CAP);
    return 0;
}
