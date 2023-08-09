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

#include <stdint.h>
#include <stdbool.h>

/** 字节 */
typedef uint8_t byte;
/** 字 */
typedef uint16_t word;
/** 双字 */
typedef uint32_t dword;
/** 四字 */
typedef uint64_t qword;

/** 8位 */
typedef uint8_t b8;
/** 16位 */
typedef uint16_t b16;
/** 32位 */
typedef uint32_t b32;
/** 64位 */
typedef uint64_t b64;

// 获取机器位数
#if BITS==16
/** 机器位数 */
typedef b16 MachineBits;
#elif BITS==32
/** 机器位数 */
typedef b32 MachineBits;
#elif BITS==64
/** 机器位数 */
typedef b64 MachineBits;
#else
/** 机器位数 */
typedef b64 MachineBits;
#endif

/** 段寄存器 */
typedef b16 SegmentRegister;
/** 寄存器 */
typedef MachineBits Register;
/** 机器位数 */
typedef MachineBits MBits;