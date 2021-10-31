; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/boot/stage2/stage2_entry.asm
;
; Stage entry here
;



[SECTION .text]
[BITS 16]

global _start
extern entry

_start:
    mov ax, cs
    mov ds, ax
    push word 0
    call entry
    jmp $