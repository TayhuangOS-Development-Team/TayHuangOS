/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * init_int.h
 *
 * 初始化中断
 *
 */



#pragma once

#include <tayhuang/defs.h>

struct pushad_regs {
    b64 rflags,
        r15,
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
        rsp,
        rbp;
};

PUBLIC void init_pic(void);
PUBLIC void disable_irq(_IN int irq);
PUBLIC void enable_irq(_IN int irq);
PUBLIC void send_eoi(_IN int irq);
PUBLIC void init_idt(void);
typedef void(*int_handler)(void);