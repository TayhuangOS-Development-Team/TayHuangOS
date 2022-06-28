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
 * int_handlers.c
 *
 * 中断处理函数
 *
 */



#include "int_handlers.h"
#include "printf.h"
#include <tayhuang/int_vectors.h>
#include <tayhuang/ports.h>
#include <tayhuang/io.h>

PUBLIC void exception_handler(int vector_no, int errcode, int eip, int cs, int eflags) {
    byte old_print_color = get_print_color();
    const char *exception_msg[] = { //异常信息
        "[#DE] Divide by 0 error!\n",
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
    printf ("Oops!There is a exception throwing!The followings are its information:\n"); //
    if (vector_no < 32) {
        printf ("Message: %s", exception_msg[vector_no]); //打印信息
    }
    printf ("Vector = %d", vector_no); //打印Vector号
    if (errcode != 0xFFFFFFFF) {
        printf (";\tError code = %d", errcode); //打印错误码
    }

    printf ("\n");
    printf ("\n");
    printf ("Registers:\n");
    int _eax, _ebx, _ecx, _edx, _esi, _edi, _esp, _ebp;
    asmv ("movl %%eax, %0" : "=m"(_eax));
    asmv ("movl %%ebx, %0" : "=m"(_ebx));
    asmv ("movl %%ecx, %0" : "=m"(_ecx));
    asmv ("movl %%edx, %0" : "=m"(_edx));
    asmv ("movl %%esi, %0" : "=m"(_esi));
    asmv ("movl %%edi, %0" : "=m"(_edi));
    asmv ("movl %%esp, %0" : "=m"(_esp));
    asmv ("movl %%ebp, %0" : "=m"(_ebp)); //获取寄存器
    printf ("eax: %#08X ; ebx: %#08X\n", _eax, _ebx);
    printf ("ecx: %#08X ; edx: %#08X\n", _ecx, _edx);
    printf ("esi: %#08X ; edi: %#08X\n", _esi, _edi);
    printf ("esp: %#08X ; ebp: %#08X\n", _esp, _ebp);
    printf ("cs:  %#04X     ;  ds: %#04X\n", cs, rdds());
    printf ("eip: %#08X;eflags:%#08X\n", eip, eflags); //打印寄存器
    set_print_color(old_print_color);
}

PRIVATE irq_handler IRQ_HANDLER_LIST[16] = {};

PUBLIC void register_irq_handler(int irq, irq_handler handler) {
    IRQ_HANDLER_LIST[irq] = handler;
}

PUBLIC void enable_irq(int irq) {
    if (irq < 8) {
        byte i = inb(M_PIC_DATA); //主片
        i &= ~(1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA); //从片
        i &= ~(1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
}

PUBLIC void disable_irq(int irq) {
    if (irq < 8) {
        byte i = inb(M_PIC_DATA); //主片
        i |= (1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA); //从片
        i |= (1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
}

PUBLIC void irq_interrup_handler(int irq) {
    disable_irq(irq); //禁用IRQ

    outb(M_PIC_CONTROL, 0x20); //EOI
    outb(DELAY_PORT, 0);
    outb(S_PIC_CONTROL, 0x20); //EOI

    asmv ("sti"); //CPU会自动关中断

    if (IRQ_HANDLER_LIST[irq] != NULL) { //存在对应的IRQ处理器
        IRQ_HANDLER_LIST[irq](irq);
    }

    asmv ("cli"); //恢复原来状态

    enable_irq(irq); //启用IRQ
}