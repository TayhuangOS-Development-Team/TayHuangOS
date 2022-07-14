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
 * video主函数
 *
 */



#include <debug/logging.h>

#include <assert.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>

#include <tayhuang/services.h>

PUBLIC void kmod_main(void) {
    set_logging_name("Video");
    linfo ("233");

    bool status = true;
    send_msg(status, sizeof(bool), INIT_SERVICE);
    
    check_ipc();

    while (true);
}