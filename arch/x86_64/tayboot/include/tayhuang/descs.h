// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------Reference from linux----------------------------------
 *
 * kernel/boot/include/tayhuang/descs.h
 *
 * Desc macro & structures here
 */



#pragma once

#include <tay_defs.h>

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

struct ldttss_desc {
	b16	limit0;
	b16	base0;

	b16	base1 : 8, type : 5, dpl : 2, p : 1;
	b16	limit1 : 4, zero0 : 3, g : 1, base2 : 8;
#ifdef ARCH_x86_64
	b32	base3;
	b32	zero1;
#endif
} __attribute__((packed));

typedef struct ldttss_desc ldt_desc;
typedef struct ldttss_desc tss_desc;

struct idt_bits {
	b16		ist	: 3,
			zero	: 5,
			type	: 5,
			dpl	: 2,
			p	: 1;
} __attribute__((packed));

struct idt_data {
	unsigned int	vector;
	unsigned int	segment;
	struct idt_bits	bits;
	const void	*addr;
};

struct gate_struct {
	b16		offset_low;
	b16		segment;
	struct idt_bits	bits;
	b16		offset_middle;
#ifdef ARCH_x86_64
	b32		offset_high;
	b32		reserved;
#endif
} __attribute__((packed));

typedef struct gate_struct gate_desc;

struct desc_ptr {
	unsigned short size;
	unsigned long address;
} __attribute__((packed)) ;

#define AR_TYPE_RODATA (0 * (1 << 9))
#define AR_TYPE_RWDATA (1 * (1 << 9))
#define AR_TYPE_RODATA_EXPDOWN	(2 * (1 << 9))
#define AR_TYPE_RWDATA_EXPDOWN	(3 * (1 << 9))
#define AR_TYPE_XOCODE (4 * (1 << 9))
#define AR_TYPE_XRCODE 5 * (1 << 9))
#define AR_TYPE_XOCODE_CONF	(6 * (1 << 9))
#define AR_TYPE_XRCODE_CONF	(7 * (1 << 9))
#define AR_TYPE_MASK (7 * (1 << 9))

#define AR_DPL0 (0 * (1 << 13))
#define AR_DPL3 (3 * (1 << 13))
#define AR_DPL_MASK	(3 * (1 << 13))

#define AR_A (1 << 8)   /* "Accessed" */
#define AR_S (1 << 12)  /* If clear, "System" segment */
#define AR_P (1 << 15)  /* "Present" */
#define AR_AVL (1 << 20)  /* "AVaiLable" (no HW effect) */
#define AR_L (1 << 21)  /* "Long mode" for code segments */
#define AR_DB (1 << 22)  /* D/B, effect depends on type */
#define AR_G (1 << 23)  /* "Granularity" (limit in pages) */

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