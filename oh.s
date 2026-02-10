	.file	"oh.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter an octal number: "
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
	subq	$64, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	$0, -56(%rbp)
	movl	$1, -52(%rbp)
	movl	$0, -48(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-60(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	jmp	.L2
.L3:
	movl	-60(%rbp), %edx
	movslq	%edx, %rax
	imulq	$1717986919, %rax, %rax
	shrq	$32, %rax
	sarl	$2, %eax
	movl	%edx, %ecx
	sarl	$31, %ecx
	subl	%ecx, %eax
	movl	%eax, -36(%rbp)
	movl	-36(%rbp), %ecx
	movl	%ecx, %eax
	sall	$2, %eax
	addl	%ecx, %eax
	addl	%eax, %eax
	subl	%eax, %edx
	movl	%edx, -36(%rbp)
	movl	-36(%rbp), %eax
	imull	-52(%rbp), %eax
	addl	%eax, -56(%rbp)
	sall	$3, -52(%rbp)
	movl	-60(%rbp), %eax
	movslq	%eax, %rdx
	imulq	$1717986919, %rdx, %rdx
	shrq	$32, %rdx
	movl	%edx, %ecx
	sarl	$2, %ecx
	cltd
	movl	%ecx, %eax
	subl	%edx, %eax
	movl	%eax, -60(%rbp)
.L2:
	movl	-60(%rbp), %eax
	testl	%eax, %eax
	jne	.L3
	cmpl	$0, -56(%rbp)
	jne	.L6
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L12
.L9:
	movl	-56(%rbp), %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$28, %eax
	addl	%eax, %edx
	andl	$15, %edx
	subl	%eax, %edx
	movl	%edx, -40(%rbp)
	cmpl	$9, -40(%rbp)
	jg	.L7
	movl	-40(%rbp), %eax
	leal	48(%rax), %ecx
	movl	-48(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -48(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
	jmp	.L8
.L7:
	movl	-40(%rbp), %eax
	leal	55(%rax), %ecx
	movl	-48(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -48(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
.L8:
	movl	-56(%rbp), %eax
	leal	15(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$4, %eax
	movl	%eax, -56(%rbp)
.L6:
	cmpl	$0, -56(%rbp)
	jne	.L9
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-48(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -44(%rbp)
	jmp	.L10
.L11:
	movl	-44(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	subl	$1, -44(%rbp)
.L10:
	cmpl	$0, -44(%rbp)
	jns	.L11
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %eax
.L12:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L13
	call	__stack_chk_fail@PLT
.L13:
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
