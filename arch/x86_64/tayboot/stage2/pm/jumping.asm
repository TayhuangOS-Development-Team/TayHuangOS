; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage2/pm/jumping.asm
;
; The finally jump here
;



[SECTION .text]
[BITS 16]
global the_finally_jump
the_finally_jump: ;void the_finally_jump(dword entrypoint, sreg_t cs_selector, sreg_t ds_selector, sreg_t tss_selector, void* boot_args)
    xor ebx, ebx
    mov bx, cs
    shl ebx, 4
    add ebx, after_the_jump
    mov dword [.jmp_pos], ebx ;与intcall相同的dirty hack

    mov cx, word [esp + 4] ;cx -> cs_selector
    mov word [.jmp_seg], cx ;cs -> cs_selector
    mov cx, word [esp + 8] ;cx -> ds_selector
    mov di, word [esp + 12] ;di -> tss_selector
    mov edx, dword [esp + 16] ;edx -> boot_args

    mov ebx, cr0
    or ebx, 1
    mov cr0, ebx

;跳转指令
;之所以不用jmp dword
;是因为我们的CS是平坦4GB描述符
;需要对目标地址作一些计算
db 0x66
db 0xEA
.jmp_pos: dd 0
.jmp_seg: dw 0

[SECTION .text32]
[BITS 32]
after_the_jump:
    mov ds, cx
    mov es, cx
    mov fs, cx
    mov gs, cx
    mov ss, cx
    mov esp, eax
    sub esp, 0x20
    mov ebp, esp

    ltr di ;欺骗CPU

    mov ebx, edx

    xor ecx, ecx
    xor edx, edx
    xor esi, esi
    xor edi, edi

    lldt dx ;欺骗CPU

    jmp eax