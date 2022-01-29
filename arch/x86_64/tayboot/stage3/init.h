// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage3/init.h
 *
 * Initialize functions are declared here
 */



#pragma once

#include <descs.h>

typedef void(*int_handler)(void);

#define GDT_SIZE (16)
#define IDT_SIZE (16)

extern struct desc_struct GDT[GDT_SIZE];
extern struct gdt_ptr gdtr;
extern gate_desc IDT[IDT_SIZE];
extern struct desc_ptr idtr;

PUBLIC void init_gdt(void);
PUBLIC void init_8259A(void);
PUBLIC void init_idt_desc(byte vector, byte type, int_handler handler, byte privilege);
PUBLIC void init_idt(void);