// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/disk.h
 *
 * Global header here
 */

#pragma once

#ifdef _WIN32
#define ed_callasm()
#define cpu_relax()
#else
#define ed_callasm() asm("addw $2, %sp")
#define cpu_relax() asm("rep; nop")
#endif

#include "types.h"

#define BASE_OF_DATA 0x1000

static inline byte inb(word port) {
    byte data;
#ifndef _WIN32
    asm volatile ("inb %1, %0" : "=a"(data) : "dN"(port));
#endif
    return data;
}

static inline void outb(word port, byte data) {
#ifndef _WIN32
    asm volatile ("outb %0, %1" : : "a"(data), "dN"(port));
#endif
}

static inline word inw(word port) {
    word data;
#ifndef _WIN32
    asm volatile ("inw %1, %0" : "=a"(data) : "dN"(port));
#endif
    return data;
}

static inline void outw(word port, word data) {
#ifndef _WIN32
    asm volatile ("outw %0, %1" : : "a"(data), "dN"(port));
#endif
}

static inline dword ind(word port) {
    dword data;
#ifndef _WIN32
    asm volatile ("inl %1, %0" : "=a"(data) : "dN"(port));
#endif
    return data;
}

static inline void outd(word port, dword data) {
#ifndef _WIN32
    asm volatile ("outl %0, %1" : : "a"(data), "dN"(port));
#endif
}

#include "memory.h"