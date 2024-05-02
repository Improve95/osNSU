#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>

void my_free(void *__ptr) {

}

void * my_malloc(size_t __size) {
    
}

int main() {
    int *a = my_malloc(10 * sizeof(int));
    my_free(a);
}
