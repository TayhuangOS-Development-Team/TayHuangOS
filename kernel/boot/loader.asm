; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage2/loader.asm
;
; Loader here
;



    org 0x0100
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x0100
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

    mov di, STAGE2_NAME
    call FindRootEntry
    cmp ax, 0xffff
    jne .goon
    mov di, MSG_NOT_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0x0C
    call DispStr
    jmp $
.goon:
    mov di, MSG_FOUND
    mov dl, 0
    mov dh, 1
    mov bl, 0xC
    call DispStr

    mov di, STAGE2_NAME
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    mov si, 0
    call LoadFile

    mov ax, cs
    mov ds, ax
    mov di, ELF_MAGIC
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    mov si, 0
    mov cx, 4
    call CMP_STR

    cmp ax, 0
    jne .valid
    mov di, MSG_ELF_NOT_MATCH
    mov dl, 0
    mov dh, 2
    mov bl, 0xC
    call DispStr
    jmp $
.valid:
    call InitStage2
    jmp far [STAGE2_ADDR]

InitStage2:
    mov ax, BASE_OF_STAGE2_DATA
    mov es, ax
    call GetELFHeaderInfo

    xor edi, edi
    mov edi, dword [ELFHeaderInfo + 4]

    mov cx, word [ELFHeaderInfo + 16]
.lp:
    cmp cx, 0
    jle .ed
    call GetProgramHeaderInfo

    mov eax, dword [ProgramHeaderInfo + 12]
    cmp eax, 1
    jne .tail

    push edi
    push esi
    push ecx
    push ds
    push es

    mov esi, dword[ProgramHeaderInfo + 4]
    mov edi, dword[ProgramHeaderInfo + 0]
    mov ecx, dword[ProgramHeaderInfo + 8]
    mov ax, BASE_OF_STAGE2
    mov es, ax
    mov ax, BASE_OF_STAGE2_DATA
    mov ds, ax

    call MemCpy

    pop es
    pop ds
    pop ecx
    pop esi
    pop edi

.tail:

    add di, word [ELFHeaderInfo + 14]
    dec cx
    jmp .lp
.ed:
    mov eax, dword [ELFHeaderInfo + 0]
    mov word [STAGE2_ADDR + 0], ax
    ret

STAGE2_ADDR:
    dw 0
    dw BASE_OF_STAGE2

GetELFHeaderInfo:
    mov eax, dword [es:0x18]
    mov dword [ELFHeaderInfo + 0], eax

    mov eax, dword [es:0x1c]
    mov dword [ELFHeaderInfo + 4], eax

    mov eax, dword [es:0x20]
    mov dword [ELFHeaderInfo + 8], eax

    mov ax, word [es:0x28]
    mov word [ELFHeaderInfo + 12], ax

    mov ax, word [es:0x2a]
    mov word [ELFHeaderInfo + 14], ax

    mov ax, word [es:0x2c]
    mov word [ELFHeaderInfo + 16], ax

    mov ax, word [es:0x2e]
    mov word [ELFHeaderInfo + 18], ax

    mov ax, word [es:0x30]
    mov word [ELFHeaderInfo + 20], ax
    ret

GetProgramHeaderInfo: ;edi = offset
    mov eax, dword [es:(edi + 4)]
    mov dword [ProgramHeaderInfo + 0], eax

    mov eax, dword [es:(edi + 12)]
    mov dword [ProgramHeaderInfo + 4], eax

    mov eax, dword [es:(edi + 16)]
    mov dword [ProgramHeaderInfo + 8], eax

    mov eax, dword [es:(edi + 0)]
    mov dword [ProgramHeaderInfo + 12], eax
    ret

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

BASE_OF_STAGE2_DATA equ 0x7000
BASE_OF_STAGE2 equ 0x6000
OFFSET_OF_STAGE2 equ 0x0220
MSG_LOADING: db `Loading...\00`
MSG_NOT_FOUND: db `NotFound!\00`
MSG_FOUND: db `Found!\00`
MSG_ELF_NOT_MATCH: db `Invalid Stage2: ELF Magic Number not match!\00`
STAGE2_NAME: db "STAGE2  BIN"

ELF_MAGIC: db `\x7fELF`
%include "functions.inc"