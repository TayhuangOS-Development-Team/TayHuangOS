; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; kernel/entry.asm
;
; K内核入口
;



[SECTION .text]

global _start
extern entry

_start:
    mov ax, 0xB800
    mov gs, ax
    mov ah, 0x0C
    mov al, 'K' ;打印'K'
    mov word [gs:((80 * 4 + 39) * 2)], ax
    ;call entry
    jmp $