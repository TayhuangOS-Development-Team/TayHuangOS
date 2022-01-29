// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/include/tayhuang/tay_mem.h
 *
 * Basic macros ard defined here
 */



#pragma once

#include <tay_type.h>
#include <tay_mem.h>

#define PUBLIC
#define PRIVATE static
#define HIGHBYTE(x) ((byte)((x) >> 8))
#define LOWBYTE(x) ((byte)x)
#define HIGHHEX(x) ((x) >> 4)
#define LOWHEX(x) ((x) & 0xF)
#define HIGHWORD(x) ((word)((x) >> 16))
#define LOWWORD(x) ((word)(x))
#define MKBYTE(h, l) ((h) << 4 + (l))
#define MKWORD(h, l) ((((word)h) << 8) + (l))
#define MKDWORD(h, l) ((((dword)h) << 16) + (l))
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define LOWBIT(x) ((x) & (-(x)))
#define TO2POW(num, x) (((num) + (x) - 1) & (~((x) - 1)))
#define abs(x) ((x) < 0 ? (-(x)) : (x))

static inline byte inb(word port) {
    byte data;
    asmv ("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outb(word port, byte data) {
    asmv ("outb %0, %1" : : "a"(data), "dN"(port));
}

static inline word inw(word port) {
    word data;
    asmv ("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outw(word port, word data) {
    asmv ("outw %0, %1" : : "a"(data), "dN"(port));
}

static inline dword ind(word port) {
    dword data;
    asmv ("inl %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outd(word port, dword data) {
    asmv ("outl %0, %1" : : "a"(data), "dN"(port));
}

static inline void io_delay(void) {
    outb(0x80, 0);
}
