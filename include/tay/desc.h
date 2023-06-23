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

// 特权级
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
	// 数据段
	DTYPE_RODATA                  = 0x0,
	DTYPE_RODATA_ACCESSED         = 0x1,
	DTYPE_RWDATA                  = 0x2,
	DTYPE_RWDATA_ACCESSED         = 0x3,
	DTYPE_RODATA_EXPDOWN          = 0x4,
	DTYPE_RODATA_EXPDOWN_ACCESSED = 0x5,
	DTYPE_RWDATA_EXPDOWN          = 0x6,
	DTYPE_RWDATA_EXPDOWN_ACCESSED = 0x7,
	// 代码段
	DTYPE_XOCODE                  = 0x8,
	DTYPE_XOCODE_ACCESSED         = 0x9,
	DTYPE_XRCODE                  = 0xA,
	DTYPE_XRCODE_ACCESSED         = 0xB,
	DTYPE_XOCODE_CONF             = 0xC,
	DTYPE_XOCODE_CONF_ACCESSED    = 0xD,
	DTYPE_XRCODE_CONF             = 0xE,
	DTYPE_XRCODE_CONF_ACCESSED    = 0xF,
	// GATE TYPES
	// 286
	GTYPE_286_TSS                 = 0x1,
	GTYPE_LDT                     = 0x2,
	GTYPE_286_TSS_BUSY            = 0x3,
	GTYPE_286_CALL_GATE           = 0x4,
	GTYPE_TASK_GATE               = 0x5,
	GTYPE_286_INT_GATE            = 0x6,
	GTYPE_286_TRAP_GATE           = 0x7,
	// 386
	GTYPE_386_TSS                 = 0x9,
	GTYPE_386_TSS_BUSY            = 0xB,
	GTYPE_386_CALL_GATE           = 0xC,
	GTYPE_386_INT_GATE            = 0xE,
	GTYPE_386_TRAP_GATE           = 0xF
};

/**
 * @brief 描述符(未处理)
 * 
 */
typedef struct {
	dword limit;
	dword base;
	word dpl;
	word type;
	bool system; // =0 => system ; =1 => data/code
	bool present;
	bool avl;
	bool lm;
	bool db;
	bool granulity;
} raw_desc_t;

/**
 * @brief 描述符
 * 
 */
struct desc_struct {
	word limit0;
	word base0;
	word base1 : 8;
	word type : 4;
	bool system: 1;
	word dpl: 2;
	bool present: 1;
	word limit1: 4;
	bool avl: 1;
	bool lm: 1;
	bool db: 1;
	bool granulity: 1;
	word base2: 8;
} __attribute__((packed));

typedef struct desc_struct desc_t;

/**
 * @brief 处理描述符
 * 
 * @param raw 未处理描述符
 * @return 已处理描述符
 */
inline static desc_t build_descriptor(raw_desc_t raw) {
	desc_t desc;

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

/**
 * @brief 描述符表指针
 * 
 */
struct desc_ptr {
	word size;
	qword address;
} __attribute__((packed));

typedef struct desc_ptr dptr_t;

/**
 * @brief TSS项
 * 
 */
struct tss_struct {
	word	 limit0;
	word	 base0;

	byte	 base1 : 8;
	byte   type : 5;
	byte   dpl : 2;
	bool present : 1;
	byte	 limit1 : 4;
	byte   reserved0 : 3;
	bool g : 1;
	word  base2 : 8;
#if BITS == 64
	dword	 base3;
	dword	 reserved1;
#endif
} __attribute__((packed));

typedef struct tss_struct tss_desc_t;

/**
 * @brief IDT属性
 * 
 */
struct idt_attr {
	byte ist       : 3;
	byte reserved  : 5;
	byte type      : 5;
	byte dpl       : 2;
	bool present : 1;
} __attribute__((packed));

typedef struct idt_attr idt_attr_t;

/**
 * @brief 门描述符
 * 
 */
struct gate_struct {
	word	   offset0;
	word	   segment;
	idt_attr_t bits;
	word	   offset1;
#if BITS == 64
    dword	   offset2;
	dword	   reserved;
#endif
} __attribute__((packed));

typedef struct gate_struct gate_desc_t;

#if BITS == 32

/**
 * @brief 构建门描述符
 * 
 * @param type 类型
 * @param ptr 地址
 * @param privilege 特权级
 * @param cs 代码段描述符
 * @return 门描述符 
 */
inline static gate_desc_t build_gate(byte type, void *ptr, byte privilege, int cs) {
    dword base = (dword)ptr;
	gate_desc_t desc = {};
    desc.offset0 = base & 0xFFFF; //偏移
    desc.segment = cs; //段
    desc.bits.ist = 0;
    desc.bits.reserved = 0;
    desc.bits.type = type; //类型
    desc.bits.dpl = privilege; //权限
    desc.bits.present = true; //存在
    desc.offset1 = base >> 16; //偏移
	return desc;
}

#elif BITS == 64

/**
 * @brief 构建门描述符
 * 
 * @param type 类型
 * @param ptr 地址
 * @param privilege 特权级
 * @param cs 代码段描述符
 * @return 门描述符 
 */
inline static gate_desc_t build_gate(byte type, void *ptr, byte privilege, int cs) {
    dword base = (dword)ptr;
	gate_desc_t desc = {};
    desc.offset0 = base & 0xFFFF; //偏移
    desc.segment = cs; //段
    desc.bits.ist = 0;
    desc.bits.reserved = 0;
    desc.bits.type = type; //类型
    desc.bits.dpl = privilege; //权限
    desc.bits.present = true; //存在
    desc.offset1 = (base >> 16) & 0xFFFF; //偏移
    desc.offset2 = base >> 32; //偏移
    desc.reserved = 0;
}

#endif

/**
 * @brief 64位TSS
 * 
 */
struct tss {
	dword reserved0;
	// RSP
	qword rsp0;
	qword rsp1;
	qword rsp2;
	qword reserved1;
	// IST
	qword ist1;
	qword ist2;
	qword ist3;
	qword ist4;
	qword ist5;
	qword ist6;
	qword ist7;
	qword reserved2;
	word  reserved3;
	// IO Permission Bitmap
	word  iopb;
} __attribute__((packed));