/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * disk.c
 *
 * 硬盘操作
 *
 */



#include "disk.h"
#include <tool/io.h>

#define CMOS_RAM_BASE    (0x70)
#define CMOS_RAM_ADDRESS (CMOS_RAM_BASE + 0)
#define CMOS_RAM_DATA    (CMOS_RAM_BASE + 1)

static inline byte read_cmos(int addr) {
    outb (CMOS_RAM_ADDRESS, 0x80 | addr);
    return inb (CMOS_RAM_DATA);
}

void load_vfs(const char *name, void *addr) {
}