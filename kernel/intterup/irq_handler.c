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
 * irq_handler.c
 *
 * IRQ处理
 *
 */



#include "irq_handler.h"
#include "init_int.h"

PUBLIC short IRQ_FLAGS[16];

PRIVATE irq_handler IRQ_HANDLERS[16] = {};

PUBLIC void register_irq_handler(int irq, irq_handler handler) {
    IRQ_FLAGS[irq] = 0;
    IRQ_HANDLERS[irq] = handler;
}

PUBLIC void general_irq_handler(int irq) {
    disable_irq(irq);
    asmv ("sti");

    send_eoi(irq);
    if (IRQ_HANDLERS[irq] != NULL)
        IRQ_FLAGS[irq] = IRQ_HANDLERS[irq](irq);

    asmv ("cli");
    enable_irq(irq);
}