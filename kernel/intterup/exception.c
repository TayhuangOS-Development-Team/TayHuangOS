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

struct pushad_regs {
    b64 rflags,
        r15,
        r14,
        r13,
        r12,
        r11,
        r10,
        r9,
        r8,
        rdi,
        rsi,
        rdx,
        rcx,
        rbx,
        rax,
        rsp,
        rbp;
};

PUBLIC void general_exception_handler(int vector, int errcode, long long cs, long long rip, word eflags, void *registers) {
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
    printk ("cs: %#04X;rip: %#016X;eflags:%#04X\n", cs, rip, eflags); //打印寄存器
    struct pushad_regs *regs = (struct pushad_regs*)registers;
    printk ("rax: %#016X;rbx: %#016X;rcx: %#016X;\n", regs->rax, regs->rbx, regs->rcx);
    printk ("rdx: %#016X;rsi: %#016X;rdi: %#016X;\n", regs->rdx, regs->rsi, regs->rdi);
    printk ("rsp: %#016X;rbp: %#016X;r8 : %#016X;\n", regs->rsp, regs->rbp, regs->r8);
    printk ("r9 : %#016X;r10: %#016X;r11: %#016X;\n", regs->r9, regs->r10, regs->r11);
    printk ("r12: %#016X;r13: %#016X;r14: %#016X;\n", regs->r12, regs->r13, regs->r14);
    printk ("r15: %#016X;rflags: %#016X;\n", regs->r15, regs->rflags);
    set_print_color(old_print_color);

    while (true);
}