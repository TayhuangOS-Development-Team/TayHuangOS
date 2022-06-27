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
; arch/x86_64/tayhuang/stage3/lm/lm_operators.asm
;
; 长模式相关汇编操作
;


global __set_cr0
global __get_cr0
global __set_cr2
global __get_cr2
global __set_cr3
global __get_cr3
global __set_cr4
global __get_cr4
global __set_efer
global __get_efer

__set_cr0:
    mov cr0, eax
    ret

__get_cr0:
    mov eax, cr0
    ret

__set_cr2:
    mov cr2, eax
    ret

__get_cr2:
    mov eax, cr2
    ret

__set_cr3:
    mov cr3, eax
    ret

__get_cr3:
    mov eax, cr3
    ret

__set_cr4:
    mov cr4, eax
    ret

__get_cr4:
    mov eax, cr4
    ret

__get_efer:
    push edx
    push ecx
    xor edx, edx
    mov ecx, 0xC0000080 ;EFER
    rdmsr
    pop ecx
    pop edx
    ret

__set_efer:
    push edx
    push ecx
    xor edx, edx
    mov ecx, 0xC0000080 ;EFER
    wrmsr
    pop ecx
    pop edx
    ret

global goto_ia32e
goto_ia32e:
    mov edi, dword [esp + 4] ;edx -> boot_args
    mov bx, word [esp + 8] ;bx -> selector64
    mov word [.jmp_seg], bx
    mov dword [.jmp_pos], eax
    add bx, 8
    mov ds, bx

    mov esp, eax
    sub esp, 0x20 ;设置esp
    mov ebp, esp

    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    xor esi, esi

    jmp $ ;Bug here

db 0xEA
.jmp_pos: dd 0
.jmp_seg: dw 0

%define CS_SELECTOR (1 << 3)

global flush_cs
flush_cs:
    jmp CS_SELECTOR:flush_end ;利用长跳刷新流水线并更新CS

flush_end:
    ret
    