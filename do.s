	.file	"do.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a decimal number: "
.LC1:
	.string	"%d"
.LC2:
	.string	"Octal: 0"
.LC3:
	.string	"Octal: "
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
	movl	$0, -104(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-108(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-108(%rbp), %eax
	testl	%eax, %eax
	jne	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L8
.L5:
	movl	-108(%rbp), %ecx
	movl	-104(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -104(%rbp)
	movl	%ecx, %edx
	sarl	$31, %edx
	shrl	$29, %edx
	addl	%edx, %ecx
	andl	$7, %ecx
	subl	%edx, %ecx
	movl	%ecx, %edx
	cltq
	movl	%edx, -96(%rbp,%rax,4)
	movl	-108(%rbp), %eax
	leal	7(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$3, %eax
	movl	%eax, -108(%rbp)
.L4:
	movl	-108(%rbp), %eax
	testl	%eax, %eax
	jne	.L5
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	-104(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -100(%rbp)
	jmp	.L6
.L7:
	movl	-100(%rbp), %eax
	cltq
	movl	-96(%rbp,%rax,4), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	subl	$1, -100(%rbp)
.L6:
	cmpl	$0, -100(%rbp)
	jns	.L7
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %eax
.L8:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L9
	call	__stack_chk_fail@PLT
.L9:
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
