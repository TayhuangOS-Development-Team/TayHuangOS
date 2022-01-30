; SPDX-License-Identifier: GPL-2.0-only
; -------------------------------*-TayhuangOS-*-----------------------------------
;
;   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
;
; --------------------------------------------------------------------------------
;
; 作者: Flysong
;
; arch/x86_64/tayboot/stage2/io.asm
;
; 基础的io函数
;



[SECTION .text]
[BITS 16]

;清屏函数
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
    int 0x10 ;0x10中断

    pop edx
    pop ecx
    pop ebx
    pop eax

    ret

;C下调用中断
global _intcall
_intcall: ; void _intcall(pintargs args)
    pushad
    mov bl, byte [eax + 8]
    mov byte [cs:.intn], bl ;利用dirty hack把中断号写入代码(参考自linux)
    mov dword[.ax_store], eax
    mov eax, dword [eax + 0] ; eax -> in regs

    ;设置入参
    mov ecx, dword [eax + 8]
    mov edx, dword [eax + 12]
    mov edi, dword [eax + 16]
    mov esi, dword [eax + 20]
    mov bx, word [eax + 24]
    mov es, bx
    mov bx, word [eax + 26]
    mov fs, bx
    mov bx, word [eax + 28]
    mov gs, bx
    mov ebx, dword [eax + 4]
    mov eax, dword [eax + 0]

db 0xcd ;int的opcode
.intn: db 0

    xchg eax, dword [.ax_store]

    ;获取出参
    mov eax, dword[eax + 4] ; eax -> out regs
    mov dword [eax + 4], ebx
    mov dword [eax + 8], ecx
    mov dword [eax + 12], edx
    mov dword [eax + 16], edi
    mov dword [eax + 20], esi
    push ebx
    mov bx, es
    mov word [eax + 24], bx
    mov bx, fs
    mov word [eax + 26], bx
    mov bx, gs
    mov word [eax + 28], bx
    pushf
    pop bx ;bx -> eflags
    mov word [eax + 30], bx
    mov ebx, dword [.ax_store]
    mov dword [eax + 0], ebx
    pop ebx

    popad
    ret
.ax_store: dd 0