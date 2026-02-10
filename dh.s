	.file	"dh.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a decimal number: "
.LC1:
	.string	"%d"
.LC2:
	.string	"Hexadecimal: 0"
.LC3:
	.string	"Hexadecimal: "
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -44(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-48(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-48(%rbp), %eax
	testl	%eax, %eax
	jne	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L10
.L7:
	movl	-48(%rbp), %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$28, %eax
	addl	%eax, %edx
	andl	$15, %edx
	subl	%eax, %edx
	movl	%edx, -36(%rbp)
	cmpl	$9, -36(%rbp)
	jg	.L5
	movl	-36(%rbp), %eax
	leal	48(%rax), %ecx
	movl	-44(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -44(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
	jmp	.L6
.L5:
	movl	-36(%rbp), %eax
	leal	55(%rax), %ecx
	movl	-44(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -44(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
.L6:
	movl	-48(%rbp), %eax
	leal	15(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$4, %eax
	movl	%eax, -48(%rbp)
.L4:
	movl	-48(%rbp), %eax
	testl	%eax, %eax
	jne	.L7
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-44(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -40(%rbp)
	jmp	.L8
.L9:
	movl	-40(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	subl	$1, -40(%rbp)
.L8:
	cmpl	$0, -40(%rbp)
	jns	.L9
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %eax
.L10:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L11
	call	__stack_chk_fail@PLT
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
