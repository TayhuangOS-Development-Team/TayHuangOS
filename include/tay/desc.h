/**
 * @file desc.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 描述符
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/types.h>

enum {
	DPL0 = 0,
	DPL1 = 1,
	DPL2 = 2,
	DPL3 = 3,
	DPL_S = 0,
	DPL_U = 3,
	DPL_KERNEL = 0,
	DPL_USER = 3
};

enum {
	// DESCRIPTOR TYPES
	DTYPE_RODATA                  = 0x0,
	DTYPE_RODATA_ACCESSED         = 0x1,
	DTYPE_RWDATA                  = 0x2,
	DTYPE_RWDATA_ACCESSED         = 0x3,
	DTYPE_RODATA_EXPDOWN          = 0x4,
	DTYPE_RODATA_EXPDOWN_ACCESSED = 0x5,
	DTYPE_RWDATA_EXPDOWN          = 0x6,
	DTYPE_RWDATA_EXPDOWN_ACCESSED = 0x7,
	DTYPE_XOCODE                  = 0x8,
	DTYPE_XOCODE_ACCESSED         = 0x9,
	DTYPE_XRCODE                  = 0xA,
	DTYPE_XRCODE_ACCESSED         = 0xB,
	DTYPE_XOCODE_CONF             = 0xC,
	DTYPE_XOCODE_CONF_ACCESSED    = 0xD,
	DTYPE_XRCODE_CONF             = 0xE,
	DTYPE_XRCODE_CONF_ACCESSED    = 0xF,
	// GATE TYPES
	GTYPE_286_TSS                 = 0x1,
	GTYPE_LDT                     = 0x2,
	GTYPE_286_TSS_BUSY            = 0x3,
	GTYPE_286_CALL_GATE           = 0x4,
	GTYPE_TASK_GATE               = 0x5,
	GTYPE_286_INT_GATE            = 0x6,
	GTYPE_286_TRAP_GATE           = 0x7,
	GTYPE_386_TSS                 = 0x9,
	GTYPE_386_TSS_BUSY            = 0xB,
	GTYPE_386_CALL_GATE           = 0xC,
	GTYPE_386_INT_GATE            = 0xE,
	GTYPE_386_TRAP_GATE           = 0xF
};

typedef struct {
	b32 limit;
	b32 base;
	b16 dpl;
	b16 type;
	bool system; // =0 => system ; =1 => data/code
	bool present;
	bool avl;
	bool lm;
	bool db;
	bool granulity;
} raw_desc_t;

struct desc_struct {
	b16	 limit0;
	b16	 base0;
	b16	 base1 : 8;
	b16 type : 4;
	bool system: 1;
	b16  dpl: 2;
	bool present: 1;
	b16	 limit1: 4;
	bool avl: 1;
	bool lm: 1;
	bool db: 1;
	bool granulity: 1;
	b16 base2: 8;
} __attribute__((packed));

typedef struct desc_struct descriptor_t;

inline static descriptor_t build_descriptor(raw_desc_t raw) {
	descriptor_t desc;

	desc.limit0 = raw.limit;
	desc.limit1 = (raw.limit >> 16) & 0xF;

	desc.base0 = raw.base & 0xFFFF;
	desc.base1 = (raw.base >> 16) & 0xFF;
	desc.base2 = (raw.base >> 24) & 0xFF;

	desc.dpl = raw.dpl;
	desc.type = raw.type;

	desc.system = raw.system;
	desc.present = raw.present;
	desc.avl = raw.avl;
	desc.lm = raw.lm;
	desc.db = raw.db;
	desc.granulity = raw.granulity;

	return desc;
}

struct desc_ptr {
	word size;
	qword address;
} __attribute__((packed));

typedef struct desc_ptr dptr_t;

#ifndef LOADER32BIT

struct tss_struct {
	b16	limit0;
	b16	base0;

	b16	base1 : 8, type : 5, dpl : 2;
	bool p : 1;
	b16	limit1 : 4, zero0 : 3;
	bool g : 1;
	b16 base2 : 8;
	b32	base3;
	b32	zero1;
} __attribute__((packed));//引用自linux

#else

struct tss_struct {
	b16	limit0;
	b16	base0;

	b16	base1 : 8, type : 5, dpl : 2;
	bool p : 1;
	b16	limit1 : 4, zero0 : 3;
	bool g : 1;
	b16 base2 : 8;
} __attribute__((packed));//引用自linux

#endif

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

#ifndef LOADER32BIT

struct _gate_struct {
	b16		offset_low;
	b16		segment;
	struct idt_bits	bits;
	b16		offset_middle;
    b32		offset_high;
	b32		reserved;
} __attribute__((packed));//引用自linux

#else

struct _gate_struct {
	b16		offset_low;
	b16		segment;
	struct idt_bits	bits;
	b16		offset_middle;
} __attribute__((packed));//引用自linux

#endif

typedef struct _gate_struct gate_desc;

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