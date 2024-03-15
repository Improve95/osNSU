#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    while(1) {
        printf("pid process: %d\n", getpid());
        sleep(2);
    }

    return 0;
}
