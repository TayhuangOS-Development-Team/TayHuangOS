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
 * tty主函数
 * 
 */



#include <tayhuang/defs.h>
#include <debug/logging.h>

PUBLIC void kmod_init(void) {
    set_logging_name("TTY");
    linfo ("Hi!I'm TTY!");
}

PUBLIC void kmod_main(void) {
}