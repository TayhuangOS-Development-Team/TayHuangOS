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
 * tools.c
 *
 * 工具函数
 *
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

//随机数生成器
PRIVATE dword _random(dword seed) { 
    if (seed == 0) return 0x12345678;
    dword a = seed * seed % 123;
    dword b = seed * seed * seed % 456;
    dword c = (seed + 7890) % 123;
    dword x = (seed + 456) * (seed + 789);
    return a * x * x + b * x + c;
}

PUBLIC int random(dword seed, int min, int max) {
    dword step = max - min;
    return _random(seed) % step + min;
}

PUBLIC char escape(const char *sentence) {
    char ch = *sentence;
    switch (ch) {
    case 'n': return '\n';
    case 'r': return '\r';
    case 'v': return '\v';
    case 't': return '\t';
    case '@': return '\"';
    case '\\': return '\\';
    case '$': return '$';
    }
    return *sentence;
}