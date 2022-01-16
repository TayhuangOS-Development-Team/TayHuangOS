// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/tools.c
 *
 * Tool functions are implemented here
 */



#include "tools.h"
#include "intcall.h"

PUBLIC dword get_clock_time(void) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = 0;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x1A;
    intcall(&args); //0x1A ah = 0中断
    return MKDWORD(out_regs.ecx, out_regs.edx);
}

PUBLIC byte bcd2num4(byte bcd) {
    return bcd;
}

PUBLIC byte bcd2num8(byte bcd) {
    return bcd2num4(bcd & 0xF) + bcd2num4(bcd >> 4) * 10;
}

PUBLIC void get_time(struct time_t *tm) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = MKWORD(2, 0);
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x1A;
    intcall(&args); //0x1A ah = 2中断
    tm->hour = bcd2num8(LOWWORD(HIGHBYTE(out_regs.ecx)));
    tm->minute = bcd2num8(LOWWORD(LOWBYTE(out_regs.ecx)));
    tm->second = bcd2num8(LOWWORD(HIGHBYTE(out_regs.edx)));
    tm->isdst = bcd2num8(LOWWORD(LOWBYTE(out_regs.edx)));
}

PUBLIC void get_date(struct date_t *dt) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = MKWORD(4, 0);
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x1A;
    intcall(&args); //0x1A ah = 4中断
    dt->year = bcd2num8(LOWWORD(HIGHBYTE(out_regs.ecx))) * 100 + bcd2num8(LOWWORD(LOWBYTE(out_regs.ecx)));
    dt->month = bcd2num8(LOWWORD(HIGHBYTE(out_regs.edx)));
    dt->day = bcd2num8(LOWWORD(LOWBYTE(out_regs.edx)));
}

PRIVATE dword _random(dword seed) { //恶臭的随机数生成
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