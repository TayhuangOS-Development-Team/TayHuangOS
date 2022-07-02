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

#include <tayhuang/io.h>

#include <intterup/exception.h>

#include <printk.h>
#include <logging.h>
#include <assert.h>

#include <tayhuang/control_registers.h>

PUBLIC int fault_num[32] = {};

PUBLIC void general_exception_handler(int vector, int errcode, long long cs, long long rip, word eflags, struct intterup_args *regs) {
    const char *exception_msg[] = { //异常信息
        "[#DE] Devide by 0 error!",
        "[#DB] Single step",
        "[None] Non-maskable interrup!",
        "[#BP] Break point",
        "[#OF] Overflow!",
        "[#BR] Bound range exceeded!",
        "[#UD] Invalid Opcode(Undefined Opcode)!",
        "[#NM] Device not available(No math coprocessor)!",
        "[#DF] Double fault!",
        "[None] Coprocessor segment overrun(Should be reserved)!",
        "[#TS] Invalid TSS!",
        "[#NP] Segment not present!",
        "[#SS] Stack segment not present!",
        "[#GP] General protection fault!",
        "[#PF] Page fault!",
        "[Reserved] Reserved exception!",
        "[#MF] x87 floating-point exception(Math fault)!",
        "[#AC] Alignment check!",
        "[#MC] Machine check!",
        "[#XF] SIMD floating-point exception!",
        "[#VE] Virtualization exception!",
        "[#CP] Control protection exception!",
        "[Reserved] Reserved exception!",
        "[Reserved] Reserved exception!",
        "[Reserved] Reserved exception!",
        "[Reserved] Reserved exception!",
        "[Reserved] Reserved exception!",
        "[Reserved] Reserved exception!",
        "[#HV] Hypervisor injection exception!",
        "[#VC] VMM communication exception!",
        "[#SX] Security exception!",
        "[Reserved] Reserved exception!"
    };

    char buffer[128];

    lerror ("Exception", "-------------------------------------");
    lerror ("Exception", "Oops!There is a exception throwing!The followings are its information:"); //
    if (vector < 32) {
        sprintk (buffer, "Message: %s", exception_msg[vector]); //打印信息
        lerror ("Exception", buffer);
    }
    sprintk (buffer, "Vector = %d", vector); //打印Vector号
    lerror ("Exception", buffer);
    if (errcode < 0xFFFFFFFF) {
        sprintk (buffer, "Error code = %d", errcode); //打印错误码
        lerror ("Exception", buffer);
    }

    lerror ("Exception", "");
    lerror ("Exception", "");
    lerror ("Exception", "Registers:");
    sprintk (buffer, "cs: %#04X;rip: %#016X;eflags:%#04X", cs, rip, eflags); //打印寄存器
    lerror ("Exception", buffer);

    sprintk (buffer, "rax: %#016X;rbx: %#016X;rcx: %#016X;", regs->rax, regs->rbx, regs->rcx);
    lerror ("Exception", buffer);
    sprintk (buffer, "rdx: %#016X;rsi: %#016X;rdi: %#016X;", regs->rdx, regs->rsi, regs->rdi);
    lerror ("Exception", buffer);
    sprintk (buffer, "rsp: %#016X;rbp: %#016X;r8 : %#016X;", regs->rsp, regs->rbp, regs->r8);
    lerror ("Exception", buffer);
    sprintk (buffer, "r9 : %#016X;r10: %#016X;r11: %#016X;", regs->r9, regs->r10, regs->r11);
    lerror ("Exception", buffer);
    sprintk (buffer, "r12: %#016X;r13: %#016X;r14: %#016X;", regs->r12, regs->r13, regs->r14);
    lerror ("Exception", buffer);
    sprintk (buffer, "r15: %#016X;pgd: %#016X", regs->r15, regs->pgd);
    lerror ("Exception", buffer);

    fault_num[vector] ++;

    if (fault_num[vector] >= 32) {
        lfatal ("Exception", "TOO MUCH FAULT!");
        panic ("TOO MUCH FAULT!VECTOR = %d", vector);
    }

    while (true);
}