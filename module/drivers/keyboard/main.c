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
 * main.c
 *
 * 键盘驱动主程序
 *
 */



#include <debug/logging.h>

void kmod_main(void) {
    set_logging_name("Keyboard");

    linfo ("Hello TayhuangOS! I'm keyboard driver");

    while (true);
}