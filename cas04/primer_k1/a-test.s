	.file	"test.c"
	.intel_syntax noprefix
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB0:
	.cfi_startproc
	endbr64
	sub	rsp, 72
	.cfi_def_cfa_offset 80
	mov	rdi, rsp
	mov	r8d, DWORD PTR 132[rsp]
	mov	ecx, 9
	mov	edx, DWORD PTR 96[rsp]
	mov	esi, 2
	call	bar@PLT
	mov	eax, DWORD PTR 28[rsp]
	add	eax, DWORD PTR 44[rsp]
	add	rsp, 72
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	foo, .-foo
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
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
