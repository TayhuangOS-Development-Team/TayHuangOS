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
 * __keyboard_driver_fn.c
 * 
 * keyboard_driver 函数功能
 * 
 */



#include <export/keyboard/__keyboard_driver_fn.h>

#include <tayhuang/services.h>

#include <syscall/rpc.h>
#include <memory/malloc.h>

#include <string.h>

PUBLIC key_t getkey(void) {
    char buffer[GETKEY_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, bool, false);

    key_t ret = remote_call(GETKEY_RETURN_TYPE, KEYBOARD_DRIVER_SERVICE, GETKEY_FN, MAKE_ARGS(buffer, GETKEY_ARGS_SIZE));

    return ret;
}