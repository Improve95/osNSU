#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void *libHandle = dlopen("./dhLib.so", RTLD_LAZY);

    if (libHandle) {
        void (*dynamicHello)() = dlsym(libHandle, "dynamicHello");

        if (dynamicHello) {
            dynamicHello();
        } else {
            fprintf(stderr, "Error: %s\n", dlerror());
        }

        dlclose(libHandle);
    } else {
        fprintf(stderr, "Error: %s\n", dlerror());
    }

    return 0;
}