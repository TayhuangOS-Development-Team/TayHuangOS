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
 * tty 主函数
 *
 */



#include <debug/logging.h>

void kmod_main(void) {
    set_logging_name("TTY");

    linfo ("TTY Here!");
    while(1);
}