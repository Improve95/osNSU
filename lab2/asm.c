#include <stdio.h>

int main() {
    asm(
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "mov %[message], %%rsi\n"
        "mov $13, %%rdx\n"
        "syscall\n"

        "mov $60, %%rax\n"
        "xor %%rdi, %%rdi\n"
        "syscall\n"
        :
        : [message] "i" ("Hello, world!\n\n")
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    return 0;
}
