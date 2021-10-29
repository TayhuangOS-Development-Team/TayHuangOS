[SECTION .text]
[BITS 16]

global _start
extern entry

_start:
    mov eax, 0xB800
    mov gs, ax
    mov ah, 0x0C
    mov al, 'S'
    mov word [gs:((80 * 4 + 39) * 2)], ax
    push word 0
    call entry
    jmp $