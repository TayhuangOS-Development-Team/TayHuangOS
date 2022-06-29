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
 * setup主函数
 *
 */



#include <syscall/syscall.h>
#include <debug/logging.h>
#include <ipc/ipc.h>
#include <tool/tostring.h>
#include <disk.h>
#include <fs/common.h>
#include <printf.h>

void kmod_main(void) {
    set_logging_name("Setup");

    char buffer[256] = {};

    char mod_name[64] = {};

    int kernel = 0;
    recv_any_msg_and_wait(&kernel); //获取内核进程pid


    sprintf (buffer, "Kernel PID: %d", kernel);
    linfo (buffer);

    void *context = load_fs(DISK_SEL_IDE0_MASTER, 0);

    while (true) {
        recv_msg (mod_name, kernel); //获取模块名

        sprintf (buffer, "Module Name: %s", mod_name);
        linfo (buffer);

        void *mod_addr;
        recv_msg (&mod_addr, kernel); //获取加载地址

        sprintf (buffer, "Module Load Address: %P", mod_addr);
        linfo (buffer);

        bool status = false;
        int times = 5;
        while (times --) {
            status = load_file(context, mod_name, mod_addr, false);
            if (status) break;
        }

        send_msg (&status, kernel, 1, 20); //通知内核
    }

    terminate_fs(context);

    while (1);
}