section .data
    msg db 'Hello, world!'
    len equ $ - msg

    global _main
    section .text
    
_main:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall

    mov rax, 60
    xor rdi, rdi
    syscall
