;
; SPDX-License-Identifier: LGPL-2.1-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: theflysong
;
; handlers.asm
;
; 中断处理函数
;



[SECTION .text]
[BITS 32]

jmppack:
    .address: dd 0
    .segment: dw 0

global flush_cs
; ARCHITECTURE_BASED ASSEMBLY void flush_cs(word cs_sel);
flush_cs:
    mov word [jmppack.segment], ax
    mov dword [jmppack.address], .flush_end
    jmp far [jmppack]
.flush_end:
    ret
