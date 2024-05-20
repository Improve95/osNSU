	.file	"t3.c"
	.text
	.section	.rodata
.LC0:
	.string	"%p\n"
.LC1:
	.string	"hello-%d"
.LC2:
	.string	"end of recursion"
	.text
	.globl	recursion
	.type	recursion, @function
recursion:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	-36(%rbp), %eax
	testl	%eax, %eax
	jle	.L2
	leaq	-36(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-36(%rbp), %edx
	leaq	-18(%rbp), %rax
	movl	%edx, %ecx
	leaq	.LC1(%rip), %rdx
	movl	$10, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf@PLT
	movl	-36(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -36(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	recursion
	jmp	.L3
.L2:
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L3:
	movl	$9999, %eax
	movq	-8(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	recursion, .-recursion
	.section	.rodata
	.align 8
.LC3:
	.string	"\nin child process: pid %d, ppid %d\n"
	.text
	.globl	child_func
	.type	child_func, @function
child_func:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movl	$10, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	call	recursion
	call	getppid@PLT
	movl	%eax, %ebx
	call	getpid@PLT
	movl	%ebx, %edx
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
.L7:
	movl	$5, %edi
	call	sleep@PLT
	jmp	.L7
	.cfi_endproc
.LFE7:
	.size	child_func, .-child_func
	.section	.rodata
	.align 8
.LC4:
	.string	"/home/opp112/os/osNSU/lab5/task3/stack"
.LC5:
	.string	"file open"
.LC6:
	.string	"ftruncate"
.LC7:
	.string	"clone"
	.align 8
.LC8:
	.string	"in parent process pid %d, child pid %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$2, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -20(%rbp)
	cmpl	$-1, -20(%rbp)
	jne	.L9
	leaq	.LC5(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L9:
	movl	$1048576, -16(%rbp)
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movl	-20(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	ftruncate@PLT
	cmpl	$-1, %eax
	jne	.L10
	leaq	.LC6(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L10:
	movl	-16(%rbp), %eax
	cltq
	movl	-20(%rbp), %edx
	movl	$0, %r9d
	movl	%edx, %r8d
	movl	$1, %ecx
	movl	$3, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	mmap@PLT
	movq	%rax, -8(%rbp)
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	addq	%rdx, %rax
	movl	$0, %ecx
	movl	$512, %edx
	movq	%rax, %rsi
	leaq	child_func(%rip), %rdi
	movl	$0, %eax
	call	clone@PLT
	movl	%eax, -12(%rbp)
	cmpl	$-1, -12(%rbp)
	jne	.L11
	leaq	.LC7(%rip), %rdi
	call	perror@PLT
	movl	$1, %edi
	call	exit@PLT
.L11:
	call	getpid@PLT
	movl	%eax, %ecx
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	movl	%ecx, %esi
	leaq	.LC8(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-12(%rbp), %eax
	movl	$0, %edx
	movl	$0, %esi
	movl	%eax, %edi
	call	waitpid@PLT
	movl	-16(%rbp), %eax
	movslq	%eax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	munmap@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
