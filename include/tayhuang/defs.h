/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * defs.h
 *
 * 基础的定义
 *
 */

#pragma once

#include <tayhuang/types.h>

#define PUBLIC                                        //公共
#define PRIVATE static                                //私有
#define HIGHBYTE(x) (((x) >> 8) & 0xFF)               //高字节
#define LOWBYTE(x) (x & 0xFF)                         //低字节
#define HIGHHEX(x) (((x) >> 4) & 0xF)                 //高16进制位
#define LOWHEX(x) ((x)&0xF)                           //低16进制位
#define HIGHWORD(x) ((word)((x) >> 16) & 0xFFFF)      //高字
#define LOWWORD(x) ((word)(x)&0xFFFF)                 //低字
#define MKBYTE(h, l) ((h) << 4 + (l))                 //字节
#define MKWORD(h, l) ((((word)h) << 8) + (l))         //字
#define MKDWORD(h, l) ((((dword)h) << 16) + (l))      //双字
#define max(a, b) (((a) > (b)) ? (a) : (b))           //取大
#define min(a, b) (((a) < (b)) ? (a) : (b))           //取小
#define LOWBIT(x) ((x) & (-(x)))                      //取最低bit
#define TO2POW(num, x) (((num) + (x)-1) & (~((x)-1))) //向上取为2的n次幂
#define abs(x) ((x) < 0 ? (-(x)) : (x))               //绝对值
#define _IN
#define _OUT

static inline dword leading_zeros(_IN dword x)
{
    if (x == 0)
        return 32;
    int n = 1;
    if (x >> 16 == 0)
    {
        n += 16;
        x <<= 16;
    }
    if (x >> 24 == 0)
    {
        n += 8;
        x <<= 8;
    }
    if (x >> 28 == 0)
    {
        n += 4;
        x <<= 4;
    }
    if (x >> 30 == 0)
    {
        n += 2;
        x <<= 2;
    }
    n -= x >> 31;
    return n;
}