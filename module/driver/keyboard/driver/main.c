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

#include <tayhuang/services.h>

#include <fifo.h>

#include <memory/malloc.h>

#include <export/__ioman.h>
#include <export/keyboard/__keyboard_driver_fn.h>

PRIVATE void *fifo = NULL;
#define LOCAL_FIFO_SIZE (8192)
PRIVATE void *local_fifo = NULL;

PRIVATE key_t __getkey(void) {
    key_t key;
    fifo_read_bytes(local_fifo, (byte*)&key, sizeof(key_t));
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
    local_fifo = create_fifo(LOCAL_FIFO_SIZE);
    fifo = share_keybuffer(false);
    while (true) {
        key_t key;
        fifo_read_bytes(fifo, &key, sizeof(key_t));
        
        send_msg(
            (msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()},
            &key,
            sizeof(key_t),
            TTY_DRIVER_SERVICE
        ); //发送key给TTY 令其回显

        fifo_write_bytes(local_fifo, &key, sizeof(key_t));
    }
}