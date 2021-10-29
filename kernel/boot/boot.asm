    org 0x7c00
    jmp short START
    nop
    %include "fat16head.inc"
START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    call ClrScreen

    mov di, MSG_BOOTING
    mov dl, 0
    mov dh, 0
    mov bl, 0xC
    mov cx, 10
    call DispStr

    mov di, LOADER_NAME
    call FindRootEntry
    cmp ax, 0xffff
    jne .goon
    mov di, MSG_NOT_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0x0C
    mov cx, 9
    call DispStr
    jmp $
.goon:
    mov di, MSG_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0xC
    mov cx, 6
    call DispStr

    mov di, LOADER_NAME
    mov ax, BASE_OF_LOADER
    mov es, ax
    mov si, OFFSET_OF_LOADER
    call LoadFile
    jmp BASE_OF_LOADER:OFFSET_OF_LOADER

MSG_BOOTING: db `Booting...`
MSG_NOT_FOUND: db `NotFound!`
MSG_FOUND: db `Found!`

BASE_OF_LOADER equ 0x0900
OFFSET_OF_LOADER equ 0x0100
BASE_OF_DATA equ 0x0800

LOADER_NAME: db "LOADER  BIN"

disk_address_packet: db 0x10 ; Size of Packet (+0, byte)
                     db 0 ;Reversed (+1, byte)
                     db 1 ;Block Count to Transfer (+2, byte)
                     db 0 ;Reversed ;(+3, byte)
                     dw OFFSET_OF_LOADER ;Address Of Dist - Offset (+4, word)
                     dw BASE_OF_LOADER ;Address Of Dist - Segment (+6, word)
                     dd 0 ;LBA - Low 32 Bits (+8, dword)
                     dd 0 ;LBA - High 32 Bits (+12, dword)

LoadFile: ;(ds:di = FILE NAME, es:si = Dist)
    push si
    call FindRootEntry
    pop si
    push si
.READ:
    push ax
    push bx
    mov ah, 0x0E
    mov al, '.'
    mov bh, 0
    mov bl, 0x0C
    int 0x10
    pop bx
    pop ax

    mov cx, 1
    push eax
    add eax, DATA_SECTION_START
    call ReadSector
    pop eax
    call GetFatEntry
    add si, 512
    cmp ax, 0xFFFF
    je .ed
    jmp .READ
.ed:
    pop si
    ret

GetFatEntry: ;(ax = entry_no/result)
    push si
    push di
    push es
    push ax

    push ax
    mov ax, BASE_OF_DATA
    mov es, ax
    pop ax
    xor si, si
    shr ax, 8 ;equal to the sector of the fat entry in
    inc eax
    call ReadSector
    pop ax

    mov di, ax
    and di, 0x007F
    shl di, 1
    mov ax, word[es:di]

    pop es
    pop di
    pop si
    ret

FindRootEntry: ;(ds:di = FILE NAME, ax = first clus of loader, if ax equals to 0xffff, that not found)
    push es
    mov ax, BASE_OF_DATA
    mov es, ax
    mov eax, ROOT_SECTION_START
    push eax
    xor si, si
.READ_A_SECTOR:
    pop eax
    cmp eax, RootSectorNumber + ROOT_SECTION_START
    jge .NOT_FOUND
    xor si, si
    mov cx, 1
    call ReadSector
    inc eax
    push eax
    jmp .CMP_A_ENTRY
.CMP_A_ENTRY:
    call CMP_FILE_NAME
    cmp ax, 0
    jne .FOUND
    add si, 0x20
    cmp si, 512
    jge .READ_A_SECTOR
    jmp .CMP_A_ENTRY
.NOT_FOUND:
    mov ax, 0xffff
    pop es
    ret
.FOUND:
    pop eax
    mov ax, word [es:(si + 0x1A)]
    pop es
    ret

CMP_FILE_NAME: ;(ds:di = FILE NAME 1, es:si = FILE NAME 2, ax = result)
    push cx
    push di
    push si

    mov cx, 11
.goon:
    mov al, byte[ds:di]
    mov ah, byte[es:si]
    cmp al, ah
    jne .diff
    dec cx
    cmp cx, 0
    je .same
    inc di
    inc si
    jmp .goon
.same:
    mov ax, 1
    jmp .ed
.diff:
    mov ax, 0
.ed:
    pop si
    pop di
    pop cx
    ret

ReadSector: ;(es:si = Dist, ax = Sector No, cl = Sector Num)
    push si
    push ax
    push dx
    push ebx
    push cx

    mov byte [disk_address_packet + 2], cl
    mov cx, es
    mov word [disk_address_packet + 6], cx
    mov word [disk_address_packet + 4], si
    mov word [disk_address_packet + 8], ax
    xor ebx, ebx
    mov ah, 0x42
    mov dl, 0x80
    mov si, disk_address_packet
    int 0x13

    pop cx
    pop ebx
    pop dx
    pop ax
    pop si
    ret

ClrScreen:
    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10
    ret

DispStr: ;(ds:di=str, cx = len, dl = x, dh = y, bl = color)
    push es
    push bp
    push ax

    mov ax, ds
    mov es, ax
    mov bp, di

    mov bh, 0

    mov ax, 0x1301

    int 10h

    pop ax
    pop bp
    pop es
    ret

times 510 - ($ - $$) db 0
dw 0xAA55