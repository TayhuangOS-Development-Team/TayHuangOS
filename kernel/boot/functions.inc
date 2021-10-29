BASE_OF_DATA equ 0x0800

disk_address_packet: db 0x10 ; Size of Packet (+0, byte)
                     db 0 ;Reversed (+1, byte)
                     db 1 ;Block Count to Transfer (+2, byte)
                     db 0 ;Reversed ;(+3, byte)
                     dw 0 ;Address Of Dist - Offset (+4, word)
                     dw 0 ;Address Of Dist - Segment (+6, word)
                     dd 0 ;LBA - Low 32 Bits (+8, dword)
                     dd 0 ;LBA - High 32 Bits (+12, dword)

LoadFile: ;(ds:di = FILE NAME, es:si = Dist)
    call FindRootEntry
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
    add si, 512
    call GetFatEntry
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

FindRootEntry: ;(ds:di = FILE NAME, ax = first clus of file, if ax equals to 0xffff, that not found)
    push si
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
    pop si
    ret
.FOUND:
    pop eax
    mov ax, word [es:(si + 0x1A)]
    pop es
    pop si
    ret

CMP_FILE_NAME: ;(ds:di = FILE NAME 1, es:si = FILE NAME 2, ax = result)
    push cx

    mov cx, 11
    call CMP_STR

    pop cx
    ret

CMP_STR: ;(ds:di = STR1, es:si = STR2, cx = len, ax = result)
    push cx
    push di
    push si

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
    push ax
    push bx
    push cx
    push dx

    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10

    pop dx
    pop cx
    pop bx
    pop ax
    ret

DispStr: ;(ds:di=str, dl = x, dh = y, bl = color)
    push es
    push bp
    push cx
    push ax

    call Sten
    mov cx, ax

    mov ax, ds
    mov es, ax
    mov bp, di

    mov bh, 0

    mov ax, 0x1301

    int 10h

    pop ax
    pop cx
    pop bp
    pop es
    ret

Sten: ;(ds:di=str, ax=out)
    push di
    push cx

    mov cx, 0
.goon:
    mov al, byte [ds:di]
    cmp al, 0
    je .tail
    inc cx
    inc di
    jmp .goon
.tail:
    mov ax, cx

    pop cx
    pop di
    ret

DispHex2B: ;(ax = data, bh = color, bl = disp_0x, dl = x, dh = y)
    push dx
    push ax
    mov al, ah
    mov ah, bh
    call DispHex1B
    add dl, 2
    cmp bl, 0
    je .e
    add dl, 2
.e:
    pop ax
    push ax
    mov ah, bh
    mov bl, 0
    call DispHex1B
    pop ax
    pop dx
    ret

DispHexSeq: ;(ds:di: seq, bx = number, ah = color, dh = y)
    push cx
    push dx

    xor cx, cx
    xor dl, dl

.s:
    cmp cx, bx
    je .e

    mov al, byte [di]
    push bx
    mov bl, 0
    call DispHex1B
    pop bx

    add dl, 2
    mov al, ' '
    call DispChar
    inc dl
    cmp dl, 0x30
    jne .e2
    mov dl, 0
    inc dh
.e2:
    inc di
    inc cx
    jmp .s
.e:
    pop dx
    pop cx
    ret

DispHex1B: ;(ah = color, al = data, dl = x, dh = y, bl = disp_0x)
    push dx
    cmp bl, 0
    je .disp_hex
    push ax
    mov al, '0'
    call DispChar
    inc dl
    mov al, 'x'
    call DispChar
    inc dl
    pop ax
.disp_hex:
    push ax
    shr al, 4
    cmp al, 9
    jg .disp_alpha1
    add al, '0'
    call DispChar
    jmp .disp_hex2
.disp_alpha1:
    add al, ('A'-10)
    call DispChar
.disp_hex2:
    inc dl
    pop ax
    and al, 0x0F
    cmp al, 9
    jg .disp_alpha2
    add al, '0'
    call DispChar
    pop dx
    ret
.disp_alpha2:
    add al, ('A'-10)
    call DispChar
    pop dx
    ret

DispChar: ;(ah = color, al = data, dl = x, dh = y)
    push edi
    push eax
    push ebx

    mov ax, 0xb800
    mov gs, ax

    xor eax, eax
    xor ebx, ebx
    mov al, dh
    mov bl, dh
    shl eax, 4
    shl ebx, 6
    add eax, ebx
    xor ebx, ebx
    mov bl, dl
    add eax, ebx
    shl eax, 1
    mov edi, eax

    pop ebx
    pop eax

    mov word [gs:edi], ax
    pop edi
    ret

MemCpy: ;(ds:edi:source, es:esi:dest, ecx = len)
    push bx
    push edi
    push esi
    push ecx

.Cpy:
    cmp ecx, 0
    jle .tail

    mov bl, byte[ds:edi]
    mov byte[es:esi], bl
    dec ecx
    inc edi
    inc esi
    jmp .Cpy
.tail:

    pop ecx
    pop esi
    pop edi
    pop bx
    ret

%include "fat16head.inc"