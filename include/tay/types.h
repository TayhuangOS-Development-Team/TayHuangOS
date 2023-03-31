/**
 * @file types.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 一些自定义类型
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/marks.h>

#include <stdint.h>

typedef uint8_t byte; //字节
typedef uint16_t word; //字
typedef uint32_t dword; //双字
typedef uint64_t qword; //四字

typedef uint8_t b8; //8位
typedef uint16_t b16; //16位
typedef uint32_t b32; //32位
typedef uint64_t b64; //64位

// 获取机器位数
#if BITS==16
typedef b16 generic_bits;
#elif BITS==32
typedef b32 generic_bits;
#elif BITS==64
typedef b64 generic_bits;
#else
typedef b64 generic_bits;
#endif

typedef b16 sreg_t; //段寄存器
typedef generic_bits reg_t; //寄存器

typedef generic_bits gbits_t;