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

/**
 * @brief 特权级
 * 
 */
enum DPLList {
	/** DPL=0 */
	DPL0 = 0,
	/** DPL=1 */
	DPL1 = 1,
	/** DPL=2 */
	DPL2 = 2,
	/** DPL=3 */
	DPL3 = 3,
	/** DPL=Supervisor=0 */
	DPL_S = 0,
	/** DPL=User=3 */
	DPL_U = 3,
	/** DPL=Kernel=0 */
	DPL_KERNEL = 0,
	/** DPL=User=3 */
	DPL_USER = 3
};

/**
 * @brief 描述符类型
 * 
 */
enum DescTypes {
	/** 数据段(只读) */
	DTYPE_RODATA                  = 0x0,
	/** 数据段(只读 已访问) */
	DTYPE_RODATA_ACCESSED         = 0x1,
	/** 数据段(读写) */
	DTYPE_RWDATA                  = 0x2,
	/** 数据段(读写 已访问) */
	DTYPE_RWDATA_ACCESSED         = 0x3,
	/** 数据段(只读 向下拓展) */
	DTYPE_RODATA_EXPDOWN          = 0x4,
	/** 数据段(只读 已访问 向下拓展) */
	DTYPE_RODATA_EXPDOWN_ACCESSED = 0x5,
	/** 数据段(读写 向下拓展) */
	DTYPE_RWDATA_EXPDOWN          = 0x6,
	/** 数据段(读写 已访问 向下拓展) */
	DTYPE_RWDATA_EXPDOWN_ACCESSED = 0x7,
	/** 代码段(只执行) */
	DTYPE_XOCODE                  = 0x8,
	/** 代码段(只执行 已访问) */
	DTYPE_XOCODE_ACCESSED         = 0x9,
	/** 代码段(可读可执行) */
	DTYPE_XRCODE                  = 0xA,
	/** 代码段(可读可执行 已访问) */
	DTYPE_XRCODE_ACCESSED         = 0xB,
	/** 代码段(只执行 一致) */
	DTYPE_XOCODE_CONF             = 0xC,
	/** 代码段(只执行 已访问 一致) */
	DTYPE_XOCODE_CONF_ACCESSED    = 0xD,
	/** 代码段(可读可执行 一致) */
	DTYPE_XRCODE_CONF             = 0xE,
	/** 代码段(可读可执行 已访问 一致) */
	DTYPE_XRCODE_CONF_ACCESSED    = 0xF,
	/** 门描述符(286 TSS) */
	GTYPE_286_TSS                 = 0x1,
	/** 门描述符(LDT) */
	GTYPE_LDT                     = 0x2,
	/** 门描述符(286 TSS 忙) */
	GTYPE_286_TSS_BUSY            = 0x3,
	/** 门描述符(286 调用门) */
	GTYPE_286_CALL_GATE           = 0x4,
	/** 门描述符(任务门) */
	GTYPE_TASK_GATE               = 0x5,
	/** 门描述符(286 中断门) */
	GTYPE_286_INT_GATE            = 0x6,
	/** 门描述符(286 陷阱门) */
	GTYPE_286_TRAP_GATE           = 0x7,
	/** 门描述符(386 TSS) */
	GTYPE_386_TSS                 = 0x9,
	/** 门描述符(386 TSS 忙) */
	GTYPE_386_TSS_BUSY            = 0xB,
	/** 门描述符(386 调用门) */
	GTYPE_386_CALL_GATE           = 0xC,
	/** 门描述符(386 中断门) */
	GTYPE_386_INT_GATE            = 0xE,
	/** 门描述符(386 陷阱门) */
	GTYPE_386_TRAP_GATE           = 0xF
};

/**
 * @brief 描述符(未处理)
 * 
 */
typedef struct {
	/** 段界限 */
	dword limit;
	/** 段基址 */
	dword base;
	/** DPL */
	word dpl;
	/** 段类型 */
	word type;
	/** 是否为系统描述符 */
	bool system; // =0 => system ; =1 => data/code
	/** 存在位 */
	bool present;
	/** 自用 */
	bool avl;
	/** 长模式 */
	bool lm;
	/** D/B标志位 */
	bool db;
	/** 粒度(字节/4KB) */
	bool granulity;
} RawDesc;

/**
 * @brief 描述符
 * 
 */
struct DescStruct {
	/** 段界限0 */
	word limit0;
	/** 段基址0 */
	word base0;
	/** 段基址1 */
	word base1 : 8;
	/** 段类型 */
	word type : 4;
	/** 是否为系统描述符 */
	bool system: 1;
	/** DPL */
	word dpl: 2;
	/** 存在位 */
	bool present: 1;
	/** 段界限1 */
	word limit1: 4;
	/** 自用 */
	bool avl: 1;
	/** 长模式 */
	bool lm: 1;
	/** D/B标志位 */
	bool db: 1;
	/** 粒度(字节/4KB) */
	bool granulity: 1;
	/** 段基址2 */
	word base2: 8;
} __attribute__((packed));

typedef struct DescStruct Descriptor;

/**
 * @brief 处理描述符
 * 
 * @param raw 未处理描述符
 * @return 已处理描述符
 */
inline static Descriptor BuildDesc(RawDesc raw) {
	Descriptor desc;

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
struct DescPtr {
	/** 大小 */
	word size;
	/** 地址 */
	qword address;
} __attribute__((packed));

typedef struct DescPtr DPTR;

/**
 * @brief TSS项
 * 
 */
struct TSSStruct {
	/** 界限0 */
	word	 limit0;
	/** 基址0 */
	word	 base0;
	/** 基址1 */
	byte	 base1 : 8;
	/** TSS类型 */
	byte   type : 5;
	/** DPL */
	byte   dpl : 2;
	/** 存在位 */
	bool present : 1;
	/** 界限1 */
	byte	 limit1 : 4;
	/** 保留 */
	byte   reserved0 : 3;
	/** 粒度(字节/4KB) */
	bool g : 1;
	/** 基址2 */
	word  base2 : 8;
#if BITS == 64
	/** 基址3 */
	dword	 base3;
	/** 保留 */
	dword	 reserved1;
#endif
} __attribute__((packed));

typedef struct TSSStruct TSSDescriptor;

/**
 * @brief IDT属性
 * 
 */
struct IDTAttributeStruct {
	/** ist号 */
	byte ist       : 3;
	/** 保留 */
	byte reserved  : 5;
	/** 门类型 */
	byte type      : 5;
	/** DPL */
	byte dpl       : 2;
	/** 存在位 */
	bool present : 1;
} __attribute__((packed));

typedef struct IDTAttributeStruct IDTAttribute;

/**
 * @brief 门描述符
 * 
 */
struct GateStruct {
	/** 偏移0 */
	word	   offset0;
	/** 段 */
	word	   segment;
	/** 门属性 */
	IDTAttribute bits;
	/** 偏移1 */
	word	   offset1;
#if BITS == 64
	/** 偏移2 */
    dword	   offset2;
	/** 保留 */
	dword	   reserved;
#endif
} __attribute__((packed));

typedef struct GateStruct GateDescriptor;

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
inline static GateDescriptor BuildGate(byte type, void *ptr, byte privilege, int cs) {
    dword base = (dword)ptr;
	GateDescriptor desc = {};
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
inline static GateDescriptor BuildGate(byte type, void *ptr, byte privilege, int cs) {
    dword base = (dword)ptr;
	GateDescriptor desc = {};
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
struct TSS64Struct {
	/** 保留 */
	dword reserved0;
	/** DPL=0 RSP */
	qword rsp0;
	/** DPL=1 RSP */
	qword rsp1;
	/** DPL=2 RSP */
	qword rsp2;
	/** 保留 */
	qword reserved1;
	/** IST1 */
	qword ist1;
	/** IST2 */
	qword ist2;
	/** IST3 */
	qword ist3;
	/** IST4 */
	qword ist4;
	/** IST5 */
	qword ist5;
	/** IST6 */
	qword ist6;
	/** IST7 */
	qword ist7;
	/** 保留 */
	qword reserved2;
	/** 保留 */
	word  reserved3;
	/** IOPB */
	word  iopb;
} __attribute__((packed));

typedef struct TSS64Struct TSS64;