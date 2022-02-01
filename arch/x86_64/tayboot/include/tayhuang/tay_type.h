// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/include/tayhuang/tay_type.h
 *
 * 基础的类型
 */



#pragma once

typedef unsigned char bool; //布尔
typedef unsigned char byte; //字节
typedef unsigned short word; //字
typedef unsigned int dword; //双字
typedef unsigned long long qword; //四字
typedef byte b8; //8位
typedef word b16; //16位
typedef dword b32; //32位
typedef qword b64; //64位
typedef b16 sreg_t; //段寄存器
typedef b8 reg8_t; //8位寄存器
typedef b16 reg16_t; //16位寄存器
typedef b32 reg32_t; //32位寄存器
typedef b64 reg64_t; //64位寄存器
typedef int size_t; //size_t 大小
typedef void *handle_t; //处理器
#define true (1) //真
#define false (0) //假
#define NULL (0) //空地址

#define asmv asm volatile //简写
#define cpu_relax() asmv("rep; nop") //延迟
#ifdef REALMODE
typedef word addr_t; //地址
typedef word id_t; //编号
#define ed_callasm() asmv("addw $2, %sp") //结束asm调用(非内联)
#else
typedef void* addr_t; //地址
typedef dword id_t; //编号
#endif