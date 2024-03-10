.data

msg:
    .ascii "Hello, world!\n"
    len = . - msg

.text
    .global _start

_start:
    movq $4, %rax // номер syscall
    movq $1, %rdx // номер файлового дескриптора
    movq $msg, %rcx
    movq $len, %rdx
    int $0x80

    movq $60, %rax
    xorq %rdi, %rdi
    syscall
