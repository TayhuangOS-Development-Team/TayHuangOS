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

#include <assert.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>

#include <tayhuang/services.h>

PUBLIC void kmod_main(void) {
    set_logging_name("Setup");
    linfo ("233");

    moo();
    send_msg("Hi!I'm setup", 13, INIT_SERVICE);

    check_ipc();
}