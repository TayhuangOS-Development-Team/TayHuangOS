[SECTION .text]
[BITS 16]

global _start
extern entry

_start:
    mov ax, cs
    mov ds, ax
    push word 0
    call entry
    jmp $