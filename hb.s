	.file	"hb.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter a hexadecimal number: "
.LC1:
	.string	"%s"
.LC2:
	.string	"Binary: "
.LC3:
	.string	"0000"
.LC4:
	.string	"0001"
.LC5:
	.string	"0010"
.LC6:
	.string	"0011"
.LC7:
	.string	"0100"
.LC8:
	.string	"0101"
.LC9:
	.string	"0110"
.LC10:
	.string	"0111"
.LC11:
	.string	"1000"
.LC12:
	.string	"1001"
.LC13:
	.string	"1010"
.LC14:
	.string	"1011"
.LC15:
	.string	"1100"
.LC16:
	.string	"1101"
.LC17:
	.string	"1110"
.LC18:
	.string	"1111"
.LC19:
	.string	"\nInvalid hexadecimal digit!"
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
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, -36(%rbp)
	jmp	.L2
.L23:
	movl	-36(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	cmpl	$54, %eax
	ja	.L3
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L5(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L5(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L5:
	.long	.L20-.L5
	.long	.L19-.L5
	.long	.L18-.L5
	.long	.L17-.L5
	.long	.L16-.L5
	.long	.L15-.L5
	.long	.L14-.L5
	.long	.L13-.L5
	.long	.L12-.L5
	.long	.L11-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L10-.L5
	.long	.L9-.L5
	.long	.L8-.L5
	.long	.L7-.L5
	.long	.L6-.L5
	.long	.L4-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L3-.L5
	.long	.L10-.L5
	.long	.L9-.L5
	.long	.L8-.L5
	.long	.L7-.L5
	.long	.L6-.L5
	.long	.L4-.L5
	.text
.L20:
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L19:
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L18:
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L17:
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L16:
	leaq	.LC7(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L15:
	leaq	.LC8(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L14:
	leaq	.LC9(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L13:
	leaq	.LC10(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L12:
	leaq	.LC11(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L11:
	leaq	.LC12(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L10:
	leaq	.LC13(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L9:
	leaq	.LC14(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L8:
	leaq	.LC15(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L7:
	leaq	.LC16(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L6:
	leaq	.LC17(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L4:
	leaq	.LC18(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	jmp	.L21
.L3:
	leaq	.LC19(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	jmp	.L24
.L21:
	addl	$1, -36(%rbp)
.L2:
	movl	-36(%rbp), %eax
	cltq
	movzbl	-32(%rbp,%rax), %eax
	testb	%al, %al
	jne	.L23
	movl	$10, %edi
	call	putchar@PLT
	movl	$0, %eax
.L24:
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L25
	call	__stack_chk_fail@PLT
.L25:
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
