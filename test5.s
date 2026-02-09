	.file	"test5.c"
	.text
	.section	.rodata
.LC0:
	.string	"Starting the loop..."
.LC1:
	.string	"This will never print."
	.align 8
.LC2:
	.string	"This line will never be reached."
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
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L2
.L3:
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L2:
	movl	$1, %eax
	movl	$0, %ecx
	cltd
	idivl	%ecx
	testl	%eax, %eax
	jne	.L3
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	popq	%rbp
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
