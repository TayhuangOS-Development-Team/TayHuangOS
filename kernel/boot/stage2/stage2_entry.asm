[SECTION .text]
[BITS 16]

global _start
extern entry

_start:
    push word 0
    call entry
    jmp $