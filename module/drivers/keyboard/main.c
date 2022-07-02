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
 * main.c
 *
 * 键盘驱动主程序
 *
 */



#include <tayhuang/defs.h>

#include <debug/logging.h>
#include <memory/malloc.h>
#include <fifo.h>
#include <ipc/ipc.h>

#define KEYBUFFER_SIZE (4096)

PUBLIC void kmod_main(void) {
    set_logging_name("Keyboard");

    init_heap();

    int kernel;

    recv_any_msg_and_wait(&kernel);

    fifo_struct *key_fifo = malloc(sizeof(fifo_struct));
    void *buffer = malloc(KEYBUFFER_SIZE);

    create_fifo(key_fifo, buffer, KEYBUFFER_SIZE);

    send_msg(&key_fifo, kernel, sizeof(key_fifo), 20);
    
    while (true) {
        if (! fifo_empty(key_fifo)) {
            char ch[2];
            read_fifo(key_fifo, ch, 1);
            ch[1] = '\0';
            linfo (ch);
        }
    }

    free (key_fifo);
    free (buffer);

    while (true);
}