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
 * __tty_driver_fn.c
 * 
 * __tty_driver_fn 函数功能
 * 
 */



#include <export/tty/__tty_driver_fn.h>

#include <tayhuang/services.h>

#include <syscall/rpc.h>
#include <memory/malloc.h>

#include <string.h>

PUBLIC void *get_console(int console_no) {
    char buffer[GET_CONSOLE_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);

    void *ret = remote_call(GET_CONSOLE_RETURN_TYPE, TTY_DRIVER_SERVICE, GET_CONSOLE_FN, MAKE_ARGS(buffer, GET_CONSOLE_ARGS_SIZE));

    return ret;
}

PUBLIC void flush_console(int console_no) {
    char buffer[FLUSH_CONSOLE_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);

    remote_call(FLUSH_CONSOLE_RETURN_TYPE, TTY_DRIVER_SERVICE, FLUSH_CONSOLE_FN, MAKE_ARGS(buffer, FLUSH_CONSOLE_ARGS_SIZE));
}