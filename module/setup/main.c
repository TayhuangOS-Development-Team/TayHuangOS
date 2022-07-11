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

#include <memory/malloc.h>

#include <tayhuang/services.h>

#include <disk.h>

PUBLIC void kmod_main(void) {
    set_logging_name("Setup");

    reg_irq(14);
    reg_irq(15);

    char name[256];
    void *buffer;

    bool status = true;
    send_msg(status, sizeof(bool), INIT_SERVICE);

    while (true) {
        set_allow(INIT_SERVICE);

        check_ipc();
        assert(recv_msg(name) != -1);
        check_ipc();
        assert(recv_msg(&buffer) != -1);

        linfo ("%s:%p", name, buffer);

        assert(send_msg(&status, sizeof(bool), INIT_SERVICE));
    }

    while (true);
}