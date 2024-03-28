.data

msg:
    .ascii "Hello, world!\n"
    len = . - msg

.text
    .global _start

_start:
    movq $1, %rax // номер syscall
    movq $1, %rdi // номер файлового дескриптора
    movq $msg, %rsi
    movq $len, %rdx
    syscall

    movq $60, %rax
    xorq %rdi, %rdi
    syscall


    ; movq $1, %rdx // длина сообщения
    ; movq $msg, %rsi
    ; movq $len, %rdi
