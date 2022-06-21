/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * init.h
 *
 * 初始化函数
 *
 */



#pragma once

#include <tayhuang/descs.h>

typedef void(*int_handler)(void);

#define GDT_SIZE (16)
#define IDT_SIZE (128)

//GDT IDT表
extern struct desc_struct GDT[GDT_SIZE];
extern struct gdt_ptr gdtr;
extern gate_desc32 IDT[IDT_SIZE];
extern struct desc_ptr idtr;

//初始化函数
PUBLIC void init_gdt(void);
PUBLIC void init_pic(void);
PUBLIC void init_idt_desc(byte vector, byte type, int_handler handler, byte privilege);
PUBLIC void init_idt(void);