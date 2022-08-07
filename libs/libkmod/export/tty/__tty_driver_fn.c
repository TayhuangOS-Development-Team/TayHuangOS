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
#include <assert.h>

PUBLIC void flush_console(int console_no) {
    char buffer[FLUSH_CONSOLE_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);

    bool ret = remote_call(FLUSH_CONSOLE_RETURN_TYPE, TTY_DRIVER_SERVICE, FLUSH_CONSOLE_FN, MAKE_ARGS(buffer, FLUSH_CONSOLE_ARGS_SIZE));
    assert(ret);
}

PUBLIC bool tty_putchar(int console_no, char ch) {
    char buffer[PUTCHAR_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);
    ARG_WRITE(buf, char, ch);

    bool ret = remote_call(PUTCHAR_RETURN_TYPE, TTY_DRIVER_SERVICE, PUTCHAR_FN, MAKE_ARGS(buffer, PUTCHAR_ARGS_SIZE));
    return ret;
}

PUBLIC bool tty_puts(int console_no, const char *str) {
    int num = strlen(str);
    int size = num + PUTS_ARGS_BASE_SIZE;

    void *buffer = malloc(size);
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);
    ARG_WRITE(buf, int, num);

    memcpy(buf, str, num);

    bool ret = remote_call(PUTS_RETURN_TYPE, TTY_DRIVER_SERVICE, PUTS_FN, MAKE_ARGS(buffer, size));

    return ret;
}

PUBLIC char tty_getchar(int console_no) {
    char buffer[GETCHAR_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, int, console_no);

    char ret = remote_call(GETCHAR_RETURN_TYPE, TTY_DRIVER_SERVICE, GETCHAR_FN, MAKE_ARGS(buffer, GETCHAR_ARGS_SIZE));

    return ret;
}