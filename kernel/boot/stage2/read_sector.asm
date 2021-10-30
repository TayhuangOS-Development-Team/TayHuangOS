[SECTION .text]
[BITS 16]

disk_address_packet: db 0x10 ; Size of Packet (+0, byte)
                     db 0 ;Reversed (+1, byte)
                     db 1 ;Block Count to Transfer (+2, byte)
                     db 0 ;Reversed ;(+3, byte)
                     dw 0 ;Address Of Dist - Offset (+4, word)
                     dw 0 ;Address Of Dist - Segment (+6, word)
                     dd 0 ;LBA - Low 32 Bits (+8, dword)
                     dd 0 ;LBA - High 32 Bits (+12, dword)

global _readsector
_readsector: ;void _readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset)
    mov dword [ds:(disk_address_packet + 8)], eax
    mov ax, word [ss:(esp + 8)]
    mov word [ds:(disk_address_packet + 6)], ax
    mov ax, word [ss:(esp + 12)]
    mov word [ds:(disk_address_packet + 4)], ax
    mov eax, dword [ss:(esp + 4)]
    mov dword [ds:(disk_address_packet + 12)], eax
    xor ebx, ebx
    mov ah, 0x42
    mov dl, 0x80
    mov si, disk_address_packet
    int 0x13
    ret

global _changeds
_changeds: ;void _changeds(word segment)
    mov ds, ax
    ret

global _restoreds
_restoreds: ;void _restoreds(void)
    mov ax, cs
    mov ds, ax
    ret

global _getbyte
_getbyte: ;byte _getbyte(word segment, word offset)
    mov di, word [ss:(esp + 4)]
    push ds
    mov ds, ax
    mov al, byte [ds:di]
    pop ds
    ret

global _getword
_getword: ;word _getword(word segment, word offset)
    mov di, word [ss:(esp + 4)]
    push ds
    mov ds, ax
    mov ax, word [ds:di]
    pop ds
    ret

global _getdword
_getdword: ;dword _getdword(word segment, word offset)
    mov di, word [ss:(esp + 4)]
    push ds
    mov ds, ax
    mov eax, dword [ds:di]
    pop ds
    ret

global _setbyte
_setbyte: ;void _setbyte(word segment, word offset, byte data)
    mov di, word [ss:(esp + 4)]
    mov cl, byte [ss:(esp + 8)]

    push ds
    mov ds, ax
    mov byte [ds:di], cl
    pop ds
    ret

global _setword
_setword: ;void _setword(word segment, word offset, word data)
    mov di, word [ss:(esp + 4)]
    mov cx, word [ss:(esp + 8)]

    push ds
    mov ds, ax
    mov word [ds:di], cx
    pop ds
    ret

global _setdword
_setdword: ;void _setdword(word segment, word offset, dword data)
    mov di, word [ss:(esp + 4)]
    mov ecx, dword [ss:(esp + 8)]

    push ds
    mov ds, ax
    mov dword [ds:di], ecx
    pop ds
    ret