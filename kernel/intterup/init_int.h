/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * init_int.h
 *
 * 初始化中断
 *
 */



#pragma once

#include <tayhuang/defs.h>

struct intterup_args {
    b64 r15,
        r14,
        r13,
        r12,
        r11,
        r10,
        r9,
        r8,
        rdi,
        rsi,
        rdx,
        rcx,
        rbx,
        rax,
        gs,
        fs,
        es,
        ds,
        pgd,
        rbp;
    //
    b64 ist;
    b64 rip,
        cs,
        rflags,
        rsp,
        ss;
} __attribute__((packed));

struct exception_args {
    b64 r15,
        r14,
        r13,
        r12,
        r11,
        r10,
        r9,
        r8,
        rdi,
        rsi,
        rdx,
        rcx,
        rbx,
        rax,
        gs,
        fs,
        es,
        ds,
        pgd,
        rbp;
    //
    b64 ist;
    b64 code; 
    b64 rip,
        cs,
        rflags,
        rsp,
        ss;
} __attribute__((packed));

//初始化PIC
PUBLIC void init_pic(void);
//禁用/启用 IRQ
PUBLIC void disable_irq(int irq);
PUBLIC void enable_irq(int irq);
//发送EOI
PUBLIC void send_eoi(int irq);
//初始化IDT
PUBLIC void init_idt(void);
typedef void(*int_handler)(void);