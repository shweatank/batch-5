	.file	"fd.c"
	.text
	.section	.rodata
.LC0:
	.string	"example.txt"
	.align 8
.LC1:
	.string	"Error opening file for writing."
	.align 8
.LC2:
	.string	"Error opening file for reading."
.LC3:
	.string	"Data read from file:\n%s\n"
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
	subq	$160, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movabsq	$7358930648195425608, %rax
	movabsq	$7236828751078976617, %rdx
	movq	%rax, -144(%rbp)
	movq	%rdx, -136(%rbp)
	movabsq	$29113321670340193, %rax
	movq	%rax, -131(%rbp)
	movl	$420, %edx
	movl	$577, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -148(%rbp)
	cmpl	$0, -148(%rbp)
	jns	.L2
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L5
.L2:
	leaq	-144(%rbp), %rax
	movq	%rax, %rdi
	call	strlen@PLT
	movq	%rax, %rdx
	leaq	-144(%rbp), %rcx
	movl	-148(%rbp), %eax
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	write@PLT
	movl	-148(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	movl	$0, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	open@PLT
	movl	%eax, -148(%rbp)
	cmpl	$0, -148(%rbp)
	jns	.L4
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, %eax
	jmp	.L5
.L4:
	leaq	-112(%rbp), %rcx
	movl	-148(%rbp), %eax
	movl	$100, %edx
	movq	%rcx, %rsi
	movl	%eax, %edi
	call	read@PLT
	movl	-148(%rbp), %eax
	movl	%eax, %edi
	call	close@PLT
	leaq	-112(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC3(%rip), %rax
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
