/**
 * @file types.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 基础类型
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#pragma once

#include <stddef.h>
#include <stdbool.h>

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
typedef void *handle_t; //处理器
typedef wchar_t wchar; // wide char
typedef char nchar; // narrow char

#ifdef LOADER32BIT
typedef b32 __standard_size__;
#else
typedef b64 __standard_size__;
#endif

#define asmv asm volatile //简写

typedef __standard_size__ phy_addr_t;
typedef __standard_size__ lin_addr_t;
typedef __standard_size__ size_t;