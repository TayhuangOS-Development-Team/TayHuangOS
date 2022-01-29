; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; kernel/boot/functions.inc
;
; 实现基本的汇编函数
;



BASE_OF_DATA equ 0x0800 ;数据保存段

disk_address_packet: db 0x10 ; Packet大小 (+0, byte)
                     db 0 ; 保留 (+1, byte)
                     db 1 ; 传输扇区数 (+2, byte)
                     db 0 ; 保留 (+3, byte)
                     dw 0 ; 目标地址 - 偏移 (+4, word)
                     dw 0 ; 目标地址 - 段 (+6, word)
                     dd 0 ; LBA - 低32字节 (+8, dword)
                     dd 0 ; LBA - 高32字节 (+12, dword)

LoadFile: ;(ds:di = FILE NAME, es:si = Dist)
    call FindRootEntry 
    push si ;寻找目录项
.READ:
    push ax
    push bx
    mov ah, 0x0E ;function number = 0x0E
    mov al, '.' ;每读取一个扇区打印一个"."
    mov bh, 0
    mov bl, 0x0C ;红色
    int 0x10
    pop bx
    pop ax

    mov cx, 1
    push eax
    add eax, DATA_SECTION_START
    call ReadSector  ;读取扇区
    pop eax ;获取下个项
    add si, 512 
    call GetFatEntry ;是否结束
    cmp ax, 0xFFFF
    je .ed  ;结束
    jmp .READ  ;未结束 继续读取
.ed:
    pop si
    ret  ;退出

GetFatEntry: ;(ax = entry_no/result)
    push si
    push di
    push es
    push ax ;保护寄存器

    push ax
    mov ax, BASE_OF_DATA
    mov es, ax ;es > BASE_OF_DATA
    pop ax
    xor si, si ;si -> 0
    shr ax, 8 ;ax -> fat项所在扇区 - 1
    inc eax ;ax -> fat项所在扇区
    call ReadSector ;读取扇区
    pop ax

    mov di, ax
    and di, 0x00FF
    shl di, 1 ;dl -> fat项偏移
    mov ax, word[es:di] ;ax -> fat项

    pop es
    pop di
    pop si ;保护寄存器
    ret ;退出

FindRootEntry: ;(ds:di = FILE NAME, ax = first clus of file, if ax equals to 0xffff, that not found)
    push si
    push es ;保护寄存器
    mov ax, BASE_OF_DATA
    mov es, ax ;es -> Base of data
    mov eax, ROOT_SECTION_START
    push eax ; last sector = ROOT_SECTION_START
    xor si, si ; ax -> 0
.READ_A_SECTOR:
    pop eax ; eax -> last sector
    cmp eax, RootSectorNumber + ROOT_SECTION_START
    jge .NOT_FOUND ;eax >= Root Section End
    xor si, si ;si -> 0
    mov cx, 1 ; num = 1
    call ReadSector ;读取扇区
    inc eax ; last sector ++
    push eax ; (保存) last sector
    jmp .CMP_A_ENTRY ; 进行比较
.CMP_A_ENTRY:
    call CMP_FILE_NAME ;比较文件名
    cmp ax, 0 ;if (ax)
    jne .FOUND ;相同
    add si, 0x20 ;不同, 下个Entry
    cmp si, 512 ;Sector是否到末尾
    jge .READ_A_SECTOR ;读取扇区
    jmp .CMP_A_ENTRY ;继续比较
.NOT_FOUND:
    mov ax, 0xffff ;ax -> 0xFFFF
    pop es
    pop si ;保护寄存器
    ret ;退出
.FOUND:
    pop eax ;恢复堆栈
    mov ax, word [es:(si + 0x1A)] ;ax -> first clus
    pop es
    pop si ;保护寄存器
    ret ;退出

CMP_FILE_NAME: ;(ds:di = FILE NAME 1, es:si = FILE NAME 2, ax = result)
    push cx ; 保护寄存器

    mov cx, 11 ;size = 11
    call CMP_STR ;cmpstr(FILE NAME 1, FILE NAME 2, size)

    pop cx ; 保护寄存器
    ret ;退出

CMP_STR: ;(ds:di = STR1, es:si = STR2, cx = len, ax = result)
    push cx
    push di
    push si ; 保护寄存器
            ; loop begin
.goon:
    mov al, byte[ds:di] ;char ch1 = *(ds:di)
    mov ah, byte[es:si] ;char ch2 = *(ds:di)
    cmp al, ah
    jne .diff ; if (ch1 != ch2) goto diff
    dec cx ; len --
    cmp cx, 0
    je .same ; if (loops == 0) goto same
    inc di ; di ++
    inc si ; si ++
    jmp .goon ; continue
              ; loop end
.same:
    mov ax, 1 ;ret = true
    jmp .ed
.diff:
    mov ax, 0 ;ret = false
.ed:
    pop si
    pop di
    pop cx ; 保护寄存器
    ret ;退出

ReadSector: ;(es:si = Dist, ax = Sector No, cl = Sector Num)
    push si
    push ax
    push dx
    push ebx
    push cx ; 保护寄存器

    mov byte [disk_address_packet + 2], cl ;disk_address_packet.transfer_num = 1
    mov cx, es
    mov word [disk_address_packet + 6], cx ;disk_address_packet.dst_segment = es
    mov word [disk_address_packet + 4], si ;disk_address_packet.dst_offset = si
    mov word [disk_address_packet + 8], ax ;disk_address_packet.lba_low = ax
    xor ebx, ebx ; ebx -> 0
    mov ah, 0x42 ; function number = 0x42
    mov dl, 0x80 ; dl -> c盘驱动器号
    mov si, disk_address_packet ;si -> &disk_address_packet
    int 0x13

    pop cx
    pop ebx
    pop dx
    pop ax
    pop si ; 保护寄存器
    ret ;退出

ClrScreen:
    push ax
    push bx
    push cx
    push dx ; 保护寄存器

    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10 ;int 10h
             ;function = 0x06 (scroll up)
             ;scroll line = 0 (clear screen)
             ;rect = (0,0)->(79, 24)

    pop dx
    pop cx
    pop bx
    pop ax ; 保护寄存器
    ret ;退出

DispStr: ;(ds:di=str, dl = x, dh = y, bl = color)
    push es
    push bp
    push cx
    push ax ; 保护寄存器

    call Strlen
    mov cx, ax ;cx = strlen(str)

    mov ax, ds
    mov es, ax ;es -> ds
    mov bp, di ;bp -> di

    mov bh, 0

    mov ax, 0x1301

    int 10h ;int 10h
            ;function = 0
            ;color = color
            ;str = es:bp
            ;strlen = cx
            ;position = (dl, dh)

    pop ax
    pop cx
    pop bp
    pop es ; 保护寄存器
    ret ;退出

Strlen: ;(ds:di=str, ax=out)
    push di
    push cx ; 保护寄存器

    mov cx, 0 ;len = 0
              ;loop start
.goon:
    mov al, byte [ds:di] ;char ch = *(ds:di)
    cmp al, 0
    je .tail ; if(ch == '\0') goto tail
    inc cx ; len ++
    inc di ; di ++
    jmp .goon ;continue
              ;loop end
.tail:
    mov ax, cx ;ret = len

    pop cx
    pop di ; 保护寄存器
    ret ;退出

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