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
; arch/x86_64/tayboot/stage2/stage2_entry.asm
;
; Stage2入口
;



[SECTION .text]
[BITS 16]

global _start
extern entry

_start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax ;初始化段寄存器
    mov sp, 0x0600 ;初始化堆栈
    push word 0 ;补0，因为nasm与c的call, ret指令位长不同
    call entry
    jmp $