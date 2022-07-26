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

#include <export/__ioman.h>

PRIVATE void *fifo = NULL;

PUBLIC void kmod_main(void) {
    set_logging_name("Keyboard");

    linfo ("Hi!");

    fifo = share_keybuffer(false);

    linfo ("%p", fifo);

    check_ipc();
}