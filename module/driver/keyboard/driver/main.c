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
#include <syscall/rpc.h>

#include <fifo.h>

#include <memory/malloc.h>

#include <export/__ioman.h>
#include <export/keyboard/__keyboard_driver_fn.h>

PRIVATE void *fifo = NULL;

PRIVATE key_t __getkey(void) {
    key_t key;
    fifo_read_bytes(fifo, (byte*)&key, sizeof(key_t));
    linfo ("%c(%d, %#04X)", key, key, key);
    return key;
}

PRIVATE rpc_args_struct wrapper_getkey(int caller, rpc_func func, rpc_args_struct args) {
    key_t *result = (key_t *)malloc(sizeof(key_t *));
    *result = __getkey();

    return (rpc_args_struct){.data = result, .size = sizeof(key_t)};
}

PUBLIC void kmod_init(void) {
    set_logging_name("Keyboard");

    linfo ("Hi!");

    rpc_register(GETKEY_FN, wrapper_getkey, sizeof(GETKEY_RETURN_TYPE), GETKEY_ARGS_SIZE);
}

PUBLIC void kmod_main(void) {
    fifo = share_keybuffer(false);
}