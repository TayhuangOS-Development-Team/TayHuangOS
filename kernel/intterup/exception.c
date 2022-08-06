/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * exception.c
 *
 * 异常处理
 *
 */

#include <tayhuang/io.h>
#include <tayhuang/control_registers.h>

#include <intterup/exception.h>

#include <task/task_scheduler.h>

#include <printk.h>
#include <logging.h>
#include <global.h>

#include <assert.h>

PUBLIC qword errcode; 
//通用异常处理器
PUBLIC void general_exception_handler(int vector, struct intterup_args *regs) {
    __set_cr3(kernel_pml4);

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

    lerror ("Exception", "-------------------------------------");
    lerror ("Exception", "Oops!There is a exception throwing!The followings are its information:");
    //打印信息
    if (vector < 32) {
        lerror ("Exception", "Message: %s", exception_msg[vector]); 
    }
    //打印Vector号
    lerror ("Exception", "Vector = %d", vector); 
    if (errcode < 0xFFFFFFFFFFFFFFFF) {
        //打印错误码
        lerror ("Exception", "Error code = %d", errcode); 
    }

    lerror ("Exception", "Registers:");\
    //打印寄存器
    lerror ("Exception", "cs:  %#016X;rip: %#016X;rflags:%#016X;", regs->cs, regs->rip, regs->rflags); 
    lerror ("Exception", "rax: %#016X;rbx: %#016X;rcx:   %#016X;", regs->rax, regs->rbx, regs->rcx);
    lerror ("Exception", "rdx: %#016X;rsi: %#016X;rdi:   %#016X;", regs->rdx, regs->rsi, regs->rdi);
    lerror ("Exception", "rsp: %#016X;rbp: %#016X;r8 :   %#016X;", regs->rsp, regs->rbp, regs->r8);
    lerror ("Exception", "r9 : %#016X;r10: %#016X;r11:   %#016X;", regs->r9, regs->r10, regs->r11);
    lerror ("Exception", "r12: %#016X;r13: %#016X;r14:   %#016X;", regs->r12, regs->r13, regs->r14);
    lerror ("Exception", "r15: %#016X;pgd: %#016X;pid:   %016d;", regs->r15, regs->pgd, current_thread->task->pid);
    lerror ("Exception", "cr0: %#016X;cr2: %#016X;cr4:   %#016X;", __get_cr0(), __get_cr2(), __get_cr4());
    lerror ("Exception", "tid: %016d;", current_thread->tid);

    current_thread->state = EXCEPTION;

    after_syscall(regs);
}