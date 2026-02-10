	.file	"bh.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a binary number: "
.LC1:
	.string	"%lld"
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
	movl	$0, -60(%rbp)
	movl	$0, -56(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-40(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	jmp	.L2
.L3:
	movq	-40(%rbp), %rcx
	movabsq	$7378697629483820647, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	sarq	$2, %rdx
	movq	%rcx, %rax
	sarq	$63, %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	salq	$2, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	subq	%rax, %rcx
	movq	%rcx, %rdx
	movl	%edx, -44(%rbp)
	movl	-60(%rbp), %eax
	leal	(%rax,%rax), %edx
	movl	-44(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -60(%rbp)
	movq	-40(%rbp), %rcx
	movabsq	$7378697629483820647, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	movq	%rdx, %rax
	sarq	$2, %rax
	sarq	$63, %rcx
	movq	%rcx, %rdx
	subq	%rdx, %rax
	movq	%rax, -40(%rbp)
.L2:
	movq	-40(%rbp), %rax
	testq	%rax, %rax
	jne	.L3
	cmpl	$0, -60(%rbp)
	jne	.L6
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L12
.L9:
	movl	-60(%rbp), %edx
	movl	%edx, %eax
	sarl	$31, %eax
	shrl	$28, %eax
	addl	%eax, %edx
	andl	$15, %edx
	subl	%eax, %edx
	movl	%edx, -48(%rbp)
	cmpl	$9, -48(%rbp)
	jg	.L7
	movl	-48(%rbp), %eax
	leal	48(%rax), %ecx
	movl	-56(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -56(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
	jmp	.L8
.L7:
	movl	-48(%rbp), %eax
	leal	55(%rax), %ecx
	movl	-56(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -56(%rbp)
	movl	%ecx, %edx
	cltq
	movb	%dl, -32(%rbp,%rax)
.L8:
	movl	-60(%rbp), %eax
	leal	15(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$4, %eax
	movl	%eax, -60(%rbp)
.L6:
	cmpl	$0, -60(%rbp)
	jne	.L9
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-56(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -52(%rbp)
	jmp	.L10
.L11:
	movl	-52(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	movsbl	%al, %eax
	movl	%eax, %edi
	call	putchar@PLT
	subl	$1, -52(%rbp)
.L10:
	cmpl	$0, -52(%rbp)
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
