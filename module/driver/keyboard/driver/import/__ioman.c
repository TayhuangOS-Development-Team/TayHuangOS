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
 * __ioman.c
 * 
 * IO Manager Functions
 * 
 */



#include <export/__ioman.h>
#include <syscall/rpc.h>
#include <tayhuang/services.h>

PUBLIC void *share_keybuffer(bool flag) {
    char buffer[SHARE_KEYBUFFER_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, bool, flag);

    void *fifo = remote_call(SHARE_KEYBUFFER_RETURN_TYPE, KEYBOARD_IOMAN_SERVICE, SHARE_KEYBUFFER_FN, ((rpc_args_struct){.data = buf, .size=SHARE_KEYBUFFER_ARGS_SIZE}));

    return fifo;
}