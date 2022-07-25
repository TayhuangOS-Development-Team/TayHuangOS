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
 * main.c
 * 
 * keyboard 主函数
 * 
 */



#include <debug/logging.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>

#include <tayhuang/io.h>
#include <tayhuang/ports.h>

#include <fifo.h>

PRIVATE void *fifo = NULL;

PRIVATE void irq_handler(int irq) {
    if (irq == 1) {
        byte code = inb(KEYBOARD_8042_DATA0);
        
        if (fifo == NULL)
            return;
        
        fifo_write_bytes(fifo, &code, sizeof(byte));
    }
}

#define KEY_BUFFER_SIZE (8000)

PUBLIC void kmod_main(void) {
    set_logging_name("Keyboard");

    linfo ("Hi!");
    
    reg_irq(1);
    register_irq_handler(irq_handler);

    fifo = create_fifo(KEY_BUFFER_SIZE);

    message_loop();

    while (true);
}