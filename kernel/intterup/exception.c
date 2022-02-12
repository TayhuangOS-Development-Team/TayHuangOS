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
 * exception.c
 * 
 * 异常处理
 * 
 */



#include "exception.h"
#include "../display/printk.h"
#include <tayhuang/io.h>

PUBLIC void general_exception_handler(int vector, int errcode, long long cs, long long rip, word eflags) {
    byte old_print_color = get_print_color();
    const char *exception_msg[] = { //异常信息
        "[#DE] Devide by 0 error!\n",
        "[#DB] Single step\n",
        "[None] Non-maskable interrup!\n",
        "[#BP] Break point\n",
        "[#OF] Overflow!\n",
        "[#BR] Bound range exceeded!\n",
        "[#UD] Invalid Opcode(Undefined Opcode)!\n",
        "[#NM] Device not available(No math coprocessor)!\n",
        "[#DF] Double fault!\n",
        "[None] Coprocessor segment overrun(Should be reserved)!\n",
        "[#TS] Invalid TSS!\n",
        "[#NP] Segment not present!\n",
        "[#SS] Stack segment not present!\n",
        "[#GP] General protection fault!\n",
        "[#PF] Page fault!\n",
        "[Reserved] Reserved exception!\n",
        "[#MF] x87 floating-point exception(Math fault)!\n",
        "[#AC] Alignment check!\n",
        "[#MC] Machine check!\n",
        "[#XF] SIMD floating-point exception!\n",
        "[#VE] Virtualization exception!\n",
        "[#CP] Control protection exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[Reserved] Reserved exception!\n",
        "[#HV] Hypervisor injection exception!\n",
        "[#VC] VMM communication exception!\n",
        "[#SX] Security exception!\n", 
        "[Reserved] Reserved exception!\n"
    };

    set_print_color(0xFC);
    clrscr();
    printk ("Oops!There is a exception throwing!The followings are its information:\n"); //
    if (vector < 32) {
        printk ("Message: %s", exception_msg[vector]); //打印信息
    }
    printk ("Vector = %d", vector); //打印Vector号
    if (errcode < 0xFFFFFFFF) {
        printk (";\tError code = %d", errcode); //打印错误码
    }

    printk ("\n");
    printk ("\n");
    printk ("Registers:\n");
    printk ("cs:  %#04X ;  ds:  %#04X\n", cs, rdds());
    printk ("rip: %#016X;eflags:%#04X\n", rip, eflags); //打印寄存器
    set_print_color(old_print_color);

    while (true);
}