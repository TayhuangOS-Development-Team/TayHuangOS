; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage2/io.asm
;
; Base io functions are implemented here
;

[SECTION .text]
[BITS 16]


global _clrscr
_clrscr: ; void _clrscr(void)
    push eax
    push ebx
    push ecx
    push edx

    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0x0000
    mov dx, 0x184f
    int 0x10

    pop edx
    pop ecx
    pop ebx
    pop eax
    ret