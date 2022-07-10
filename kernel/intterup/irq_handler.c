/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * irq_handler.c
 *
 * IRQ处理
 *
 */



#include <intterup/irq_handler.h>
#include <intterup/init_int.h>

#include <tayhuang/control_registers.h>
#include <global.h>

PRIVATE irq_handler IRQ_HANDLERS[16] = {};

//注册IRQ处理器
PUBLIC void register_irq_handler(int irq, irq_handler handler) {
    IRQ_HANDLERS[irq] = handler;
}

PUBLIC bool entered_handler = false;

//通用IRQ处理器
PUBLIC void general_irq_handler(int irq, struct intterup_args *args) {
    __set_cr3(kernel_pml4);

    bool flag = entered_handler;
    if (! flag) {
        entered_handler = true;
    }

    //禁用同类中断接收
    disable_irq(irq); 

    //发送EOI
    send_eoi(irq);

    if (IRQ_HANDLERS[irq] != NULL) {
        IRQ_HANDLERS[irq](irq, args, flag);
    }

    //启用同类中断接收
    enable_irq(irq); 

    if (! flag) {
        entered_handler = false;
    }
}