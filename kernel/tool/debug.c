/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * assert.c
 * 
 * assert与panic
 * 
 */



#include <assert.h>

#include <tayhuang/defs.h>

#include <display/printk.h>

void assertion_failure(const char *expression, const char *file, const char *base_file, int line) {
    printk("assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);
    flush_to_screen();

    while (true);
    asmv ("ud2"); //不应该被执行
}

void panic_failure(const char *expression, const char *file, const char *base_file, int line) {
    dis_int(); //关中断(阻止进程切换)

    printk("panic_assert(%s) failed; file:%s ; base file: %s ; line: %d\n", expression, file, base_file, line);
    flush_to_screen();

    while (true);
    asmv ("ud2"); //不应该被执行
}

//panic在display/printk下实现