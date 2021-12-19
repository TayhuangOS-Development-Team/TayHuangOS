// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/header.h
 *
 * Global header here
 */

#pragma once

#include "types.h"

#define BASE_OF_DATA 0x1000
#define PUBLIC
#define PRIVATE static

static inline byte inb(word port) {
    byte data;
    asm volatile ("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outb(word port, byte data) {
    asm volatile ("outb %0, %1" : : "a"(data), "dN"(port));
}

static inline word inw(word port) {
    word data;
    asm volatile ("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outw(word port, word data) {
    asm volatile ("outw %0, %1" : : "a"(data), "dN"(port));
}

static inline dword ind(word port) {
    dword data;
    asm volatile ("inl %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

static inline void outd(word port, dword data) {
    asm volatile ("outl %0, %1" : : "a"(data), "dN"(port));
}

#include "memory.h"