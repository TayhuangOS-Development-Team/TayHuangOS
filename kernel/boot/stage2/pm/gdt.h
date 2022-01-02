// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/pm/gdt.h
 *
 * Gdt macro & structures here
 */


#pragma once

#include "../../header.h"

struct gdt_ptr {
	b16 len;
	b32 ptr;
} __attribute__((packed));

#define GDT_ENTRY(base, limit, flag)			\
	((((base) & 0xff000000) << (56-24)) |	\
	 (((flags) & 0x0000f0ff) << 40) |	\
	 (((limit) & 0x000f0000)) << (48-16)) |	\
	 ((((base) & 0x00ffffff)) << 16) |	\
	 (((limit) & 0x0000ffff))