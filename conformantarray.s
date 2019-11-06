	.file	"conformantarray.adb"
	.text
	.section	.rodata
.LC1:
	.ascii	"conformantarray.adb"
	.zero	1
	.text
	.align 2
	.type	conformantarray__inner.4225, @function
conformantarray__inner.4225:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, %rax
	movq	%rsi, %rcx
	movq	%rcx, %rdx
	movq	%rax, -32(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%r10, -40(%rbp)
	movq	-32(%rbp), %rax
	testq	%rax, %rax
	jne	.L2
	movl	$7, %esi
	leaq	.LC1(%rip), %rdi
	call	__gnat_rcheck_CE_Access_Check@PLT
.L2:
	movq	-24(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	jle	.L10
	movl	$1, -4(%rbp)
.L8:
	movq	-32(%rbp), %rax
	testq	%rax, %rax
	jne	.L4
	movl	$8, %esi
	leaq	.LC1(%rip), %rdi
	call	__gnat_rcheck_CE_Access_Check@PLT
.L4:
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movq	-24(%rbp), %rax
	movl	4(%rax), %eax
	cmpl	%edx, -4(%rbp)
	jl	.L5
	cmpl	%eax, -4(%rbp)
	jle	.L6
.L5:
	movl	$8, %esi
	leaq	.LC1(%rip), %rdi
	call	__gnat_rcheck_CE_Index_Check@PLT
.L6:
	movq	-24(%rbp), %rax
	movl	(%rax), %eax
	movslq	%eax, %rdx
	movq	-32(%rbp), %rax
	movl	-4(%rbp), %ecx
	movslq	%ecx, %rcx
	subq	%rdx, %rcx
	movq	%rcx, %rdx
	movl	$7, (%rax,%rdx,4)
	movl	-4(%rbp), %eax
	cmpl	-12(%rbp), %eax
	je	.L10
	addl	$1, -4(%rbp)
	jmp	.L8
.L10:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	conformantarray__inner.4225, .-conformantarray__inner.4225
	.section	.rodata
	.align 8
.LC0:
	.zero	8
	.text
	.align 2
	.globl	_ada_conformantarray
	.type	_ada_conformantarray, @function
_ada_conformantarray:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	leaq	16(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	$0, -16(%rbp)
	leaq	.LC0(%rip), %rax
	movq	%rax, -8(%rbp)
	movl	$48, %edi
	call	__gnat_malloc@PLT
	movl	$1, (%rax)
	movl	$10, 4(%rax)
	leaq	8(%rax), %rdx
	movq	%rdx, -16(%rbp)
	movq	%rax, -8(%rbp)
	movq	-16(%rbp), %rdx
	movq	-8(%rbp), %rax
	leaq	-24(%rbp), %rcx
	movq	%rcx, %r10
	movq	%rdx, %rdi
	movq	%rax, %rsi
	call	conformantarray__inner.4225
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_ada_conformantarray, .-_ada_conformantarray
	.ident	"GCC: (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0"
	.section	.note.GNU-stack,"",@progbits
