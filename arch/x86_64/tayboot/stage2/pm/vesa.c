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
 * arch/x86_64/tayboot/stage2/pm/vesa.c
 *
 * 实现vesa函数
 */



#include "vesa.h"
#include "../printf.h"

PUBLIC bool support_vesa(void) { //是否支持vesa
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    in_regs.eax = 0x4F00;
    in_regs.es = BASE_OF_DATA;
    in_regs.edi = 0;
    args.int_no = 0x10;
    intcall(&args);
    return ((word)out_regs.eax) == 0x004F; //支持
}

PUBLIC void *enable_graphic(void) { //启用图形化
    if (! support_vesa()) //是否支持vesa
        return NULL; //不支持 爬
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    in_regs.eax = 0x4F02;
    in_regs.ebx = 0x4118;
    args.int_no = 0x10;
    intcall(&args); //图形化

    in_regs.eax = 0x4F01;
    in_regs.ecx = 0x0118;
    in_regs.es = BASE_OF_DATA;
    in_regs.edi = 0;
    args.int_no = 0x10;
    intcall(&args); //获取参数

    stes(BASE_OF_DATA);
    void *addr = rdes32(40);
    return addr; //获取framebuffer
}