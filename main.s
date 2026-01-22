	.file	"main.c"
	.text
	.section	.rodata
.LC0:
	.string	"enter the choice"
.LC1:
	.string	"%c"
.LC2:
	.string	"add is %d\n"
.LC3:
	.string	"sub is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
.L6:
	movq	stdin(%rip), %rax
	movq	%rax, %rdi
	call	__fpurge@PLT
	leaq	.LC0(%rip), %rdi
	call	puts@PLT
	leaq	-13(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movzbl	-13(%rbp), %eax
	movsbl	%al, %eax
	cmpl	$101, %eax
	je	.L3
	cmpl	$115, %eax
	je	.L4
	cmpl	$97, %eax
	jne	.L2
	movl	$20, %esi
	movl	$10, %edi
	call	add@PLT
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC2(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L2
.L4:
	movl	$10, %esi
	movl	$20, %edi
	call	sub@PLT
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L2
.L3:
	movl	$0, %edi
	call	exit@PLT
.L2:
	jmp	.L6
	.cfi_endproc
.LFE5:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
