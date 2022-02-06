// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * include/tayhuang/io.h
 *
 * In&Out
 */



#pragma once

#include <tayhuang/types.h>

static inline byte inb(word port) { //in字节
    byte data;
    asmv ("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outb(word port, byte data) { //out字节
    asmv ("outb %0, %1" : : "a"(data), "dN"(port));
}

static inline word inw(word port) { //in字
    word data;
    asmv ("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outw(word port, word data) { //out字
    asmv ("outw %0, %1" : : "a"(data), "dN"(port));
}

static inline dword ind(word port) { //in双字
    dword data;
    asmv ("inl %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outd(word port, dword data) { //out双字
    asmv ("outl %0, %1" : : "a"(data), "dN"(port));
}