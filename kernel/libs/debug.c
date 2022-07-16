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
 * debug.c
 * 
 * assert与panic
 * 
 */



#include <assert.h>

#include <tayhuang/defs.h>

#include <stdarg.h>

#include <logging.h>
#include <printk.h>

void assertion_failure(const char *expression, const char *file, const char *base_file, int line) {
    lerror("assert", "assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);

    while (true);
    //不应该被执行
    asmv ("ud2"); 
}

void panic_failure(const char *expression, const char *file, const char *base_file, int line) {
    //关中断(阻止进程切换)
    dis_int();

    lerror("panic", "panic_assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);

    while (true);
    //不应该被执行
    asmv ("ud2");
}

void panic(const char *format, ...) {
    //关中断(阻止进程切换)
    dis_int(); 

    va_list args;
    va_start(args, format);

    static char buffer[256];

    vsprintk(buffer, format, args);

    lerror("panic", "%s", buffer);

    va_end(args);

    while (true);
    //不应该被执行
    asmv ("ud2");
}