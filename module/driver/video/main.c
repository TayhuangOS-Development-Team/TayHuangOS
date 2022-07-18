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
 * video主函数
 *
 */



#include <debug/logging.h>

#include <assert.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>
#include <syscall/rpc.h>

#include <memory/malloc.h>

#include <tayhuang/services.h>

#include <global.h>

PUBLIC video_info_struct video_info;

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    void *buf = msg;
    int cmdid = ARG_READ(buf, int);

    text_execute(cmdid, buf);

    set_allow(ANY_TASK);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Video");
    check_ipc();
    recv_msg(&video_info);
    
    register_normal_ipc_handler(normal_ipc_handler);
    set_allow(ANY_TASK);

    rpc_call(0, 0, (rpc_args_struct){}, 0, NULL);

    while (true);
}