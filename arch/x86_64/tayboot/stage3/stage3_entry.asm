; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage3/stage3_entry.asm
;
; Stage3 entry here
;



[SECTION .text]
[BITS 32]

global _start
extern entry

_start:
    mov eax, ebx
    call entry
    jmp $