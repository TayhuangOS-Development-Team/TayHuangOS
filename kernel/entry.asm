; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; Author: Flysong
;
; kernel/entry.asm
;
; Kernel entry here
;



[SECTION .text]

global _start
extern entry

_start:
    mov ax, 0xB800
    mov gs, ax
    mov ah, 0x0C
    mov al, 'K'
    mov word [gs:((80 * 4 + 39) * 2)], ax
    ;call entry
    jmp $