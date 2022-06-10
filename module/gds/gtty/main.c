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
 * gtty 主函数
 *
 */



#include <debug/logging.h>

void kmod_main(void) {
    set_logging_name("GTTY");

    linfo ("GTTY Here!");
    while(1);
}