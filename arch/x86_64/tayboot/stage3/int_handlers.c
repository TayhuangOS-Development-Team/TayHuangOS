// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/int_handlers.c
 *
 * 中断处理函数
 */



#include "int_handlers.h"
#include "printf.h"
#include <int_vectors.h>
#include <ports.h>

PUBLIC void exception_handler(int vector_no, int errcode, int eip, int cs, int eflags) {
    byte old_print_color = get_print_color();
    const char* exception_msg[] = {
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
    printf ("Oops!There is a exception throwing!The followings are its information:\n");
    if (vector_no < 32) {
        printf ("Message: %s", exception_msg[vector_no]);
    }
    printf ("Vector = %d", vector_no);
    if (errcode != 0xFFFFFFFF) {
        printf (";\tError code = %d", errcode);
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
    asmv ("movl %%ebp, %0" : "=m"(_ebp));
    printf ("eax: %#8X ; ebx: %#8X\n", _eax, _ebx);
    printf ("ecx: %#8X ; edx: %#8X\n", _ecx, _edx);
    printf ("esi: %#8X ; edi: %#8X\n", _esi, _edi);
    printf ("esp: %#8X ; ebp: %#8X\n", _esp, _ebp);
    printf ("cs:  %#4X     ;  ds:  %#4X\n", cs, rdds());
    printf ("eip: %#8X;eflags:%#8X\n", eip, eflags);
    set_print_color(old_print_color);
}

PRIVATE irq_handler IRQ_HANDLER_LIST[16] = {};

PUBLIC void register_irq_handler(int irq, irq_handler handler) {
    IRQ_HANDLER_LIST[irq] = handler;
}

PUBLIC void irq_interrup_handler(int irq) {
    if (irq < 8) {
        byte i = inb(M_PIC_DATA);
        i |= (1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA);
        i |= (1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
    outb(M_PIC_CONTROL, 0x20);
    io_delay();
    outb(S_PIC_CONTROL, 0x20);

    asmv ("sti");

    if (IRQ_HANDLER_LIST[irq] != NULL) {
        IRQ_HANDLER_LIST[irq](irq);
    }

    asmv ("cli");

    if (irq < 8) {
        byte i = inb(M_PIC_DATA);
        i &= ~(1 << irq);
        outb(M_PIC_DATA, i);
    }
    else {
        byte i = inb(S_PIC_DATA);
        i &= ~(1 << (irq - 8));
        outb(S_PIC_DATA, i);
    }
}