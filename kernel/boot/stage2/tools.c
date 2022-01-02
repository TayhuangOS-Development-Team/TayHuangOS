// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong Beanflame
 *
 * kernel/boot/stage2/tools.c
 *
 * Tool functions are implemented here
 */



#include "./tools.h"
#include "intcall.h"

PUBLIC dword get_clock_time(void) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = 0;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x1A;
    intcall(&args);
    return MKDWORD(out_regs.ecx, out_regs.edx);
}

PRIVATE dword _random(dword seed) {
    if (seed == 0) return 0x11451419;
    dword a = seed * seed % 114;
    dword b = seed * seed * seed % 514;
    dword c = (seed + 1919) % 810;
    dword x = (seed + 114) * (seed + 514) >> 2;
    return a * x * x + b * x + c;
}

PUBLIC int random(dword seed, int min, int max) {
    dword step = max - min;
    return _random(seed) % step + min;
}