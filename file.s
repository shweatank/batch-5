	.file	"file.c"
	.text
	.section	.rodata
.LC0:
	.string	"w"
.LC1:
	.string	"example.txt"
	.align 8
.LC2:
	.string	"Error opening file for writing."
.LC3:
	.string	"r"
	.align 8
.LC4:
	.string	"Error opening file for reading."
.LC5:
	.string	"Data read from file:\n%s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$160, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$8367736964726416712, %rax
	movabsq	$7359008686476585320, %rdx
	movq	%rax, -144(%rbp)
	movq	%rdx, -136(%rbp)
	movabsq	$7018895639239550496, %rax
	movabsq	$13061572841464942, %rdx
	movq	%rax, -130(%rbp)
	movq	%rdx, -122(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -152(%rbp)
	cmpq	$0, -152(%rbp)
	jne	.L2
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L5
.L2:
	movq	-152(%rbp), %rdx
	leaq	-144(%rbp), %rax
	movq	%rdx, %rcx
	movl	$30, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite@PLT
	movq	-152(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	.LC3(%rip), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	fopen@PLT
	movq	%rax, -152(%rbp)
	cmpq	$0, -152(%rbp)
	jne	.L4
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L5
.L4:
	movq	-152(%rbp), %rdx
	leaq	-112(%rbp), %rax
	movq	%rdx, %rcx
	movl	$30, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread@PLT
	movq	-152(%rbp), %rax
	movq	%rax, %rdi
	call	fclose@PLT
	leaq	-112(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L5:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L6
	call	__stack_chk_fail@PLT
.L6:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
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
