[SECTION .text]
[BITS 16]

global _dispchar
_dispchar: ;void _dispchar(char ch, byte color, short pos)
    push eax
    mov eax, 0xB800
    mov gs, ax
    pop eax
    mov ah, byte [ss:(esp + 4)]
    mov di, word [ss:(esp + 8)]
    shl di, 1
    mov word [gs:di], ax
    ret

global _clrscr
_clrscr: ; void _clrscr(void)
    push eax
    push ebx
    push ecx
    push edx

    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10

    pop edx
    pop ecx
    pop ebx
    pop eax
    ret