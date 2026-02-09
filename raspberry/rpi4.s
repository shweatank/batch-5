	.file	"rpi4.c"
	.text
	.globl	GPFSEL1
	.data
	.align 8
	.type	GPFSEL1, @object
	.size	GPFSEL1, 8
GPFSEL1:
	.quad	4263510020
	.globl	GPSET0
	.align 8
	.type	GPSET0, @object
	.size	GPSET0, 8
GPSET0:
	.quad	4263510044
	.globl	GPCLR0
	.align 8
	.type	GPCLR0, @object
	.size	GPCLR0, 8
GPCLR0:
	.quad	4263510056
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
	movq	GPFSEL1(%rip), %rax
	movl	(%rax), %edx
	movq	GPFSEL1(%rip), %rax
	orl	$2097152, %edx
	movl	%edx, (%rax)
.L6:
	movq	GPSET0(%rip), %rax
	movl	$131072, (%rax)
	movl	$0, -4(%rbp)
	jmp	.L2
.L3:
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
.L2:
	movl	-4(%rbp), %eax
	cmpl	$999999, %eax
	jbe	.L3
	movq	GPCLR0(%rip), %rax
	movl	$131072, (%rax)
	movl	$0, -4(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	$999999, %eax
	jbe	.L5
	jmp	.L6
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
