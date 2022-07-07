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
 * main.c
 *
 * setup主函数
 *
 */



#include <debug/logging.h>
#include <syscall/syscall.h>

PUBLIC void kmod_main(void) {
    set_logging_name("Setup");
    linfo ("233");

    moo();
}