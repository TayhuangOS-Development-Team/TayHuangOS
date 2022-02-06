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
; arch/x86_64/tayboot/stage3/lm/lm_operators.asm
;
; 长模式相关汇编操作
;


global set_cr0
global get_cr0
global set_cr2
global get_cr2
global set_cr3
global get_cr3
global set_cr4
global get_cr4
global set_efer
global get_efer

set_cr0:
    mov cr0, eax
    ret

get_cr0:
    mov eax, cr0
    ret

set_cr2:
    mov cr2, eax
    ret

get_cr2:
    mov eax, cr2
    ret

set_cr3:
    mov cr3, eax
    ret

get_cr3:
    mov eax, cr3
    ret

set_cr4:
    mov cr4, eax
    ret

get_cr4:
    mov eax, cr4
    ret

get_efer:
    push edx
    push ecx
    xor edx, edx
    mov ecx, 0xC0000080 ;EFER
    rdmsr
    pop ecx
    pop edx
    ret

set_efer:
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

    mov esp, eax
    sub esp, 0x20 ;设置esp
    mov ebp, esp

    xor eax, eax
    xor ebx, ebx
    xor ecx, ecx
    xor edx, edx
    xor esi, esi

db 0xEA
.jmp_pos: dd 0
.jmp_seg: dw 0