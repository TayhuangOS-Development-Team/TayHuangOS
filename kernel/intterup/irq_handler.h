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
 * irq_handler.h
 *
 * IRQ处理
 *
 */



#pragma once

#include <tayhuang/defs.h>

#include <intterup/init_int.h>

extern short IRQ_FLAGS[16];
typedef short(*irq_handler)(int, struct intterup_args*, bool);
//注册IRQ处理器
PUBLIC void register_irq_handler(int irq, irq_handler handler);
//通用IRQ处理器
PUBLIC void general_irq_handler(int irq, struct intterup_args *args);
//系统调用处理器
PUBLIC void syscall_int_handler(struct intterup_args *regs);

//IRQ处理器
void irq0_handler(void);
void irq1_handler(void);
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);

//系统调用处理器
void syscall_handler(void);