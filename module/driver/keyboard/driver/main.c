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
 * 主函数
 * 
 */



#include <debug/logging.h>

#include <syscall/ipc.h>

#include <fifo.h>

#include <export/__ioman.h>

PRIVATE void *fifo = NULL;

PUBLIC void kmod_init(void) {
    set_logging_name("Keyboard");

    linfo ("Hi!");
}

PUBLIC void kmod_main(void) {
    fifo = share_keybuffer(false);

    while (true) {
        key_t key;
        fifo_read_bytes(fifo, (byte*)&key, sizeof(key_t));
        linfo ("%c(%d, %#04X)", key, key, key);
    }
}