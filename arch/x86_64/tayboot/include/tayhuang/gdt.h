// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------Reference from linux----------------------------------
 *
 * kernel/boot/include/tayhuang/gdt.h
 *
 * Gdt macro & structures here
 */


#pragma once

#include "../header.h"

struct desc_struct {
	b16	limit0;
	b16	base0;
	b16	base1: 8, type: 4, s: 1, dpl: 2, p: 1;
	b16	limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
} __attribute__((packed)); //引用自linux

#define GDT_ENTRY(flags, base, limit)			\
	{							\
		.limit0		= (b16) (limit),		\
		.limit1		= ((limit) >> 16) & 0x0F,	\
		.base0		= (b16) (base),			\
		.base1		= ((base) >> 16) & 0xFF,	\
		.base2		= ((base) >> 24) & 0xFF,	\
		.type		= (flags & 0x0f),		\
		.s		= (flags >> 4) & 0x01,		\
		.dpl		= (flags >> 5) & 0x03,		\
		.p		= (flags >> 7) & 0x01,		\
		.avl		= (flags >> 12) & 0x01,		\
		.l		= (flags >> 13) & 0x01,		\
		.d		= (flags >> 14) & 0x01,		\
		.g		= (flags >> 15) & 0x01,		\
	}//引用自linux

struct gdt_ptr {
	b16 len;
	b32 ptr;
} __attribute__((packed));//引用自linux