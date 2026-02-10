	.file	"bo.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a binary number: "
.LC1:
	.string	"%s"
.LC2:
	.string	"Octal: "
.LC3:
	.string	"%d"
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
	subq	$112, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-80(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	leaq	-80(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movl	%eax, -88(%rbp)
	movl	-88(%rbp), %edx
	movslq	%edx, %rax
	imulq	$1431655766, %rax, %rax
	shrq	$32, %rax
	movq	%rax, %rcx
	movl	%edx, %eax
	sarl	$31, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	addl	%eax, %eax
	addl	%ecx, %eax
	subl	%eax, %edx
	movl	%edx, -84(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	cmpl	$0, -84(%rbp)
	je	.L2
	movl	$0, -108(%rbp)
	movl	$0, -104(%rbp)
	jmp	.L3
.L4:
	movl	-108(%rbp), %eax
	leal	(%rax,%rax), %edx
	movl	-104(%rbp), %eax
	cltq
	movzbl	-80(%rbp,%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	addl	%edx, %eax
	movl	%eax, -108(%rbp)
	addl	$1, -104(%rbp)
.L3:
	movl	-104(%rbp), %eax
	cmpl	-84(%rbp), %eax
	jl	.L4
	movl	-108(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
.L2:
	movl	-84(%rbp), %eax
	movl	%eax, -100(%rbp)
	jmp	.L5
.L8:
	movl	$0, -96(%rbp)
	movl	$0, -92(%rbp)
	jmp	.L6
.L7:
	movl	-96(%rbp), %eax
	leal	(%rax,%rax), %ecx
	movl	-100(%rbp), %edx
	movl	-92(%rbp), %eax
	addl	%edx, %eax
	cltq
	movzbl	-80(%rbp,%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	addl	%ecx, %eax
	movl	%eax, -96(%rbp)
	addl	$1, -92(%rbp)
.L6:
	cmpl	$2, -92(%rbp)
	jle	.L7
	movl	-96(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$3, -100(%rbp)
.L5:
	movl	-100(%rbp), %eax
	cmpl	-88(%rbp), %eax
	jl	.L8
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L10
	call	__stack_chk_fail@PLT
.L10:
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
