; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; kernel/boot/boot.asm
;
; 主引导记录
;



    org 0x7c00 ;BIOS要求我们这么做
    jmp short START ;FAT16要求我们这么做
    nop
    %include "fat16head.inc" ;include fat16头
START:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax ;初始化段寄存器
    mov sp, 0x7C00 ;初始化堆栈

    call ClrScreen ;清屏

    mov di, MSG_BOOTING
    mov dl, 0
    mov dh, 0
    mov bl, 0xC
    mov cx, 10
    call DispStr ;打印"Booting.." DispStr("Booting..", 0, 0, 0xC, 10)

    mov di, LOADER_NAME ;di -> Loader Name
    call FindRootEntry ;寻找Loader的目录项
    cmp ax, 0xffff ;是否有找到
    jne .goon ;找到了
    mov di, MSG_NOT_FOUND ;没找到
    mov dl, 0
    mov dh, 1
    mov bl, 0x0C
    mov cx, 9
    call DispStr ;打印"NotFound!" DispStr("NotFound!", 0, 1, 0xC, 9)
    jmp $ ;停止
.goon:
    mov di, MSG_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0xC
    mov cx, 6
    call DispStr ;打印"Found!" DispStr("Found!", 0, 1, 0xC, 6)

    mov di, LOADER_NAME ;di -> Loader Name
    mov ax, BASE_OF_LOADER
    mov es, ax ;es -> Base of Loader
    mov si, OFFSET_OF_LOADER 
    call LoadFile ; loader.bin -> Base of Loader:Offset Of Loader LoadFile("loader.bin", Base of Loader, Offset Of Loader)
    jmp BASE_OF_LOADER:OFFSET_OF_LOADER ;转至loader

MSG_BOOTING: db `Booting...`
MSG_NOT_FOUND: db `NotFound!`
MSG_FOUND: db `Found!`

BASE_OF_LOADER equ 0x0900 ;Loader所在段
OFFSET_OF_LOADER equ 0x0100 ;Loader偏移
BASE_OF_DATA equ 0x0800 ;数据所在段

LOADER_NAME: db "LOADER  BIN" ;Loader名

disk_address_packet: db 0x10 ; Packet大小 (+0, byte)
                     db 0 ; 保留 (+1, byte)
                     db 1 ; 传输扇区数 (+2, byte)
                     db 0 ; 保留 (+3, byte)
                     dw OFFSET_OF_LOADER ; 目标地址 - 偏移 (+4, word)
                     dw BASE_OF_LOADER ; 目标地址 - 段 (+6, word)
                     dd 0 ; LBA - 低32字节 (+8, dword)
                     dd 0 ; LBA - 高32字节 (+12, dword)

LoadFile: ;(ds:di = FILE NAME, es:si = Dist)
    push si
    call FindRootEntry ;寻找目录项
    pop si
    push si
.READ:
    push ax
    push bx
    mov ah, 0x0E function number = 0x0E
    mov al, '.' ;每读取一个扇区打印一个"."
    mov bh, 0
    mov bl, 0x0C ;红色
    int 0x10
    pop bx
    pop ax

    mov cx, 1
    push eax
    add eax, DATA_SECTION_START
    call ReadSector ;读取扇区
    pop eax
    call GetFatEntry ;获取下个项
    add si, 512
    cmp ax, 0xFFFF ;是否结束
    je .ed ;结束
    jmp .READ ;未结束 继续读取
.ed:
    pop si
    ret ;退出

GetFatEntry: ;(ax = entry_no/result)
    push si
    push di
    push es
    push ax ;保护寄存器

    push ax
    mov ax, BASE_OF_DATA
    mov es, ax
    pop ax
    xor si, si
    shr ax, 8 ;ax -> fat项所在扇区 - 1
    inc eax ;ax -> fat项所在扇区
    call ReadSector ;读取扇区
    pop ax

    mov di, ax
    and di, 0x00FF
    shl di, 1 ;di -> fat项偏移
    mov ax, word[es:di] ;ax -> fat项

    pop es
    pop di
    pop si ;保护寄存器
    ret ;退出

FindRootEntry: ;(ds:di = FILE NAME, ax = first clus of loader, if ax equals to 0xffff, that not found)
    push es ; 保护寄存器
    mov ax, BASE_OF_DATA
    mov es, ax ;es -> Base Of Data
    mov eax, ROOT_SECTION_START
    push eax ; last sector = ROOT_SECTION_START
    xor si, si ;si -> 0
.READ_A_SECTOR:
    pop eax ; eax -> last sector
    cmp eax, RootSectorNumber + ROOT_SECTION_START
    jge .NOT_FOUND  ;eax >= Root Section End
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
    pop es ; 保护寄存器
    ret ;退出
.FOUND:
    pop eax ; 恢复堆栈
    mov ax, word [es:(si + 0x1A)] ;ax -> first clus
    pop es ; 保护寄存器
    ret ;退出

CMP_FILE_NAME: ;(ds:di = FILE NAME 1, es:si = FILE NAME 2, ax = result)
    push cx
    push di
    push si ; 保护寄存器

    mov cx, 11 ; loops = 11
               ; loop begin
.goon:
    mov al, byte[ds:di] ;char ch1 = *(ds:di)
    mov ah, byte[es:si] ;char ch2 = *(ds:di)
    cmp al, ah
    jne .diff ; if (ch1 != ch2) goto diff
    dec cx ; loops --
    cmp cx, 0 ;
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
    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10 ;int 10h
             ;function = 0x06 (scroll up)
             ;scroll line = 0 (clear screen)
             ;rect = (0,0)->(79, 24)
    ret

DispStr: ;(ds:di=str, cx = len, dl = x, dh = y, bl = color)
    push es
    push bp
    push ax ; 保护寄存器

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
    pop bp
    pop es ; 保护寄存器
    ret ;退出

times 510 - ($ - $$) db 0 ;填充剩余部分为0
dw 0xAA55 ; 结束标志AA55