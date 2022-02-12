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

PUBLIC void init_pic(void);
PUBLIC void disable_irq(_IN int irq);
PUBLIC void enable_irq(_IN int irq);
PUBLIC void send_eoi(_IN int irq);
PUBLIC void init_idt(void);
typedef void(*int_handler)(void);