/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------Reference from linux----------------------------------
 *
 * descs.h
 *
 * 描述符的结构及宏
 *
 */



#pragma once

#include <tayhuang/defs.h>

struct desc_struct {
	b16	limit0;
	b16	base0;
	b16	base1: 8, type: 4;
	bool s: 1;
	b16 dpl: 2;
	bool p: 1;
	b16	limit1: 4;
	bool avl: 1, l: 1, d: 1, g: 1;
	b16 base2: 8;
} __attribute__((packed)); //参考自linux

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

struct tss_struct {
	b16	limit0;
	b16	base0;

	b16	base1 : 8, type : 5, dpl : 2;
	bool p : 1;
	b16	limit1 : 4, zero0 : 3;
	bool g : 1;
	b16 base2 : 8;
#ifdef ARCH_x86_64
	b32	base3;
	b32	zero1;
#endif
} __attribute__((packed));//引用自linux

typedef struct tss_struct tss_desc;

struct idt_bits {
	b16		ist	: 3,
			zero	: 5,
			type	: 5,
			dpl	: 2;
	bool	p	: 1;
} __attribute__((packed));//引用自linux

struct idt_data {
	dword	vector;
	dword	segment;
	struct idt_bits	bits;
	const void	*addr;
};//引用自linux

struct _gate_struct {
	b16		offset_low;
	b16		segment;
	struct idt_bits	bits;
	b16		offset_middle;
#ifdef ARCH_x86_64
    b32		offset_high;
	b32		reserved;
#endif
} __attribute__((packed));//引用自linux

typedef struct _gate_struct gate_desc;

struct desc_ptr {
	word size;
	qword address;
} __attribute__((packed)) ;//引用自linux

#define AR_TYPE_RODATA (0)//引用自linux
#define AR_TYPE_RWDATA (1)//引用自linux
#define AR_TYPE_RODATA_EXPDOWN	(2)//引用自linux
#define AR_TYPE_RWDATA_EXPDOWN	(3)//引用自linux
#define AR_TYPE_XOCODE (4)//引用自linux
#define AR_TYPE_XRCODE (5)//引用自linux
#define AR_TYPE_XOCODE_CONF	(6)//引用自linux
#define AR_TYPE_XRCODE_CONF	(7)//引用自linux
#define AR_TYPE_MASK (7)//引用自linux

#define AR_DPL0 (0 * (1 << 5))//引用自linux
#define AR_DPL3 (3 * (1 << 5))//引用自linux
#define AR_DPL_MASK	(3 * (1 << 5))//引用自linux

#define AR_A (1 << 0)   /* "Accessed" */
#define AR_S (1 << 4)  /* If clear, "System" segment */
#define AR_P (1 << 7)  /* "Present" */
#define AR_AVL (1 << 12)  /* "AVaiLable" (no HW effect) */
#define AR_L (1 << 13)  /* "Long mode" for code segments */
#define AR_DB (1 << 14)  /* D/B, effect depends on type */
#define AR_G (1 << 15)  /* "Granularity" (limit in pages) */

enum {
	GATE_INTERRUPT = 0xE,
	GATE_TRAP = 0xF,
	GATE_CALL = 0xC,
	GATE_TASK = 0x5,
};

enum {
	DESC_TSS = 0x9,
	DESC_LDT = 0x2,
	DESCTYPE_S = 0x10,	/* !system */
};

struct tss {
	dword reserved0;
	qword rsp0;
	qword rsp1;
	qword rsp2;
	qword reserved1;
	qword ist1;
	qword ist2;
	qword ist3;
	qword ist4;
	qword ist5;
	qword ist6;
	qword ist7;
	qword reserved2;
	word reserved3;
	word iopb;
} __attribute__((packed));