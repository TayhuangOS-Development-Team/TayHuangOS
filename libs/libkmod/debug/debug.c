/*
 * SPDX-License-Identifier: GPL-3.0-only
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

#include <printf.h>

#include <debug/logging.h>

void assertion_failure(const char *expression, const char *file, const char *base_file, int line) {
    lerror ("assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);

    while (true);
    asmv ("ud2"); //不应该被执行
}

void panic_failure(const char *expression, const char *file, const char *base_file, int line) {
    dis_int(); //关中断(阻止进程切换)

    lerror("panic_assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);

    while (true);
    asmv ("ud2"); //不应该被执行
}

void panic(const char *format, ...) {
    dis_int(); //关中断(阻止进程切换)

    va_list args;
    va_start(args, format);

    static char buffer[256];

    vsprintf (buffer, format, args);

    lerror ("%s", buffer);

    va_end(args);

    while (true);
    asmv ("ud2"); //不应该被执行
}