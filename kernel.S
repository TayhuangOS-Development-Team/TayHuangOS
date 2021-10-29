	.file	"stage2.c"
	.code16gcc
	.text
	.globl	entry
	.type	entry, @function
entry:
.LFB0:
	.cfi_startproc
	endbr32
	nop
	ret
	.cfi_endproc
.LFE0:
	.size	entry, .-entry
	.ident	"GCC: (Ubuntu 10.3.0-1ubuntu1) 10.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 4
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 4
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 4
4:
