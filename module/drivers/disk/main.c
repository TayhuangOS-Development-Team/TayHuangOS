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
 * 硬盘驱动主函数
 *
 */



#include <syscall/syscall.h>
#include <debug/logging.h>
#include <ipc/ipc.h>
#include <tool/tostring.h>
#include "disk.h"

void entry(void) {
    linfo ("DISK DRIVER!");

    char buffer[256] = {};

    char vfs_name[64] = {};
    recv_any_msg(vfs_name);
    
    linfo (vfs_name);

    void *vfs_addr;
    recv_any_msg(&vfs_addr);

    lltoa((qword)vfs_addr, buffer, 16);
    linfo (buffer);

    while (1);
}