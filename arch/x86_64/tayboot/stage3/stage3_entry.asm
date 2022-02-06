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
; arch/x86_64/tayboot/stage3/stage3_entry.asm
;
; Stage3入口
;



[SECTION .text]
[BITS 32]

global _start
extern entry

_start:
    mov eax, ebx ;传参: Boot Arg
    call entry
    jmp $