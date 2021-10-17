    org 0x0100

; %define ENABLE_GRAPH
; %define ENABLE_256C_GRAPH_MODE

%ifdef ENABLE_GRAPH
    %ifdef ENABLE_16C_GRAPH_MODE
        mov ax, 0x0012
    %endif
    %ifdef ENABLE_256C_GRAPH_MODE
        mov ax, 0x0013
    %endif
    %ifdef ENABLE_MULTI_COLOR_GRAPH_MODE
        mov ax, 0x0042
    %endif
    int 0x10
%endif

    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x0100

;     mov cx, 10
;     mov ah, 0x0C
;     mov al, 0x7f
; .c:
;     mov dx, 10
;     cmp cx, 40
;     je .d
; .a:
;     cmp dx, 40
;     je .b
;     int 0x10
;     inc dx
;     jmp .a
; .b:
;     inc cx
;     jmp .c
; .d:

    call ClrScreen

    mov di, MSG_LOADING
    mov dl, 0
    mov dh, 0
    mov bl, 0xC
    call DispStr

    jmp $

MSG_LOADING: db `Loading...\00`

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

    call StrLen
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

StrLen: ;(ds:di=str, ax=out)
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