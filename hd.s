	.file	"hd.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a hexadecimal number: "
.LC1:
	.string	"%s"
.LC2:
	.string	"Invalid hexadecimal digit!"
.LC3:
	.string	"Decimal: %d\n"
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
	movl	$0, -40(%rbp)
	movl	$0, -36(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	jmp	.L2
.L8:
	movl	-36(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	movb	%al, -41(%rbp)
	sall	$4, -40(%rbp)
	cmpb	$47, -41(%rbp)
	jle	.L3
	cmpb	$57, -41(%rbp)
	jg	.L3
	movsbl	-41(%rbp), %eax
	subl	$48, %eax
	addl	%eax, -40(%rbp)
	jmp	.L4
.L3:
	cmpb	$64, -41(%rbp)
	jle	.L5
	cmpb	$70, -41(%rbp)
	jg	.L5
	movsbl	-41(%rbp), %eax
	subl	$55, %eax
	addl	%eax, -40(%rbp)
	jmp	.L4
.L5:
	cmpb	$96, -41(%rbp)
	jle	.L6
	cmpb	$102, -41(%rbp)
	jg	.L6
	movsbl	-41(%rbp), %eax
	subl	$87, %eax
	addl	%eax, -40(%rbp)
	jmp	.L4
.L6:
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L9
.L4:
	addl	$1, -36(%rbp)
.L2:
	movl	-36(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	testb	%al, %al
	jne	.L8
	movl	-40(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
.L9:
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
