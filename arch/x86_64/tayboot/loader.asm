;
; SPDX-License-Identifier: GPL-3.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; loader.asm
;
; Loader
;



    org 0x0100 ; boot的要求
start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax ;初始化段寄存器
    mov sp, 0x0100 ;初始化堆栈

    call ClrScreen ;清屏

    mov di, MSG_LOADING
    mov dl, 0
    mov dh, 0
    mov bl, 0xC
    call DispStr ;打印"Loading.." DispStr("Loading..", 0, 0, 0xC)

    mov di, STAGE2_NAME ;di -> Stage2 Name
    call FindRootEntry ;寻找Stage2的目录项
    cmp ax, 0xffff ;是否有找到
    jne .goon ;找到了
    mov di, MSG_NOT_FOUND ;没找到
    mov dl, 0
    mov dh, 1
    mov bl, 0x0C
    call DispStr ;打印"NotFound!" DispStr("NotFound!", 0, 1, 0xC)
    jmp $ ;停止
.goon:
    mov di, MSG_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0xC
    call DispStr ;打印"Found!" DispStr("Found!", 0, 1, 0xC)

    mov di, STAGE2_NAME
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    mov si, 0
    call LoadFile ;加载Stage2文件夹

    mov ax, cs
    mov ds, ax
    mov di, ELF_MAGIC
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    mov si, 0
    mov cx, 4
    call CMP_STR ;判断是否是ELF魔数

    cmp ax, 0
    jne .valid ;是
    mov di, MSG_ELF_NOT_MATCH ;不是
    mov dl, 0
    mov dh, 2
    mov bl, 0xC
    call DispStr ;DispStr("Invalid Stage2: ELF Magic Number not match!", 0, 1, 0xC)
    jmp $ ;重启吧您
.valid:
    call InitStage2 ;初始化Stage2
    jmp far [STAGE2_ADDR] ;芜湖起飞 下一站: Stage2

InitStage2:
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    call GetELFHeaderInfo ;获取ELF头

    xor edi, edi
    mov edi, dword [ELFHeaderInfo + 4] ;edi -> e_phoff

    mov cx, word [ELFHeaderInfo + 16] ;cx -> e->phnum
.lp:
    cmp cx, 0
    jle .ed ;if (cx <= 0) goto ed
    call GetProgramHeaderInfo ;获取Program头

    mov eax, dword [ProgramHeaderInfo + 12] ;eax -> 
    cmp eax, 1
    jne .tail ;if (type != PT_LOAD) goto .tail

    push edi
    push esi
    push ecx
    push ds
    push es ;保护寄存器

    mov esi, dword[ProgramHeaderInfo + 4] ;esi->p_paddr
    mov edi, dword[ProgramHeaderInfo + 0] ;edi->p_offset
    mov ecx, dword[ProgramHeaderInfo + 8] ;ecx->p_filesz
    mov ax, BASE_OF_STAGE2
    mov es, ax ;es -> Base of Stage2
    mov ax, BASE_OF_STAGE2_DATA
    mov ds, ax ;ds -> Base of Stage2 Data

    call MemCpy ;memcpy(BASE_OF_STAGE2:p_paddr, BASE_OF_STAGE2_DATA:p_offset, p_filesz)
                ;复制到需要的地方

    pop es
    pop ds
    pop ecx
    pop esi
    pop edi ;保护寄存器
.tail:
    add di, word [ELFHeaderInfo + 14] ;下一个
    dec cx ;cx --
    jmp .lp ;继续复制
.ed:
    mov eax, dword [ELFHeaderInfo + 0]
    mov word [STAGE2_ADDR + 0], ax ;将entrypoint写入STAGE2_ADDR
    ret ;退出

STAGE2_ADDR: ;用于跳转
    dw 0 ;Offset
    dw BASE_OF_STAGE2 ;Segment

GetELFHeaderInfo:
    mov eax, dword [es:0x18]
    mov dword [ELFHeaderInfo + 0], eax ;e_entry

    mov eax, dword [es:0x1c]
    mov dword [ELFHeaderInfo + 4], eax ;e_phoff

    mov eax, dword [es:0x20]
    mov dword [ELFHeaderInfo + 8], eax ;e_shoff

    mov ax, word [es:0x28]
    mov word [ELFHeaderInfo + 12], ax ;e_ehsize

    mov ax, word [es:0x2a]
    mov word [ELFHeaderInfo + 14], ax ;e_phentsize

    mov ax, word [es:0x2c]
    mov word [ELFHeaderInfo + 16], ax ;e_phnum

    mov ax, word [es:0x2e]
    mov word [ELFHeaderInfo + 18], ax ;e_shentsize

    mov ax, word [es:0x30]
    mov word [ELFHeaderInfo + 20], ax ;e_shnum
    ret ;退出

GetProgramHeaderInfo: ;edi = offset
    mov eax, dword [es:(edi + 4)]
    mov dword [ProgramHeaderInfo + 0], eax ;p_offset

    mov eax, dword [es:(edi + 12)]
    mov dword [ProgramHeaderInfo + 4], eax ;p_paddr

    mov eax, dword [es:(edi + 16)]
    mov dword [ProgramHeaderInfo + 8], eax ;p_filesz

    mov eax, dword [es:(edi + 0)]
    mov dword [ProgramHeaderInfo + 12], eax ;p_type
    ret ;退出

ELFHeaderInfo:
    dd 0 ;e_entry(0)
    dd 0 ;e_phoff(4)
    dd 0 ;e_shoff(8)
    dw 0 ;e_ehsize(12)
    dw 0 ;e_phentsize(14)
    dw 0 ;e_phnum(16)
    dw 0 ;e_shentsize(18)
    dw 0 ;e_shnum(20)

ProgramHeaderInfo:
    dd 0 ;p_offset(0)
    dd 0 ;p_paddr(4)
    dd 0 ;p_filesz(8)
    dd 0 ;p_type(12)

BASE_OF_STAGE2_DATA equ 0x7000 ;stage2.bin段
BASE_OF_STAGE2 equ 0x6000 ;Stage2段
MSG_LOADING: db `Loading...\00`
MSG_NOT_FOUND: db `NotFound!\00`
MSG_FOUND: db `Found!\00`
MSG_ELF_NOT_MATCH: db `Invalid Stage2: ELF Magic Number not match!\00`
STAGE2_NAME: db "STAGE2  BIN" ;Stage2名

ELF_MAGIC: db `\x7fELF` ;ELF魔数
%include "functions.inc"