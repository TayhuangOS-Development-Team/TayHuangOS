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

PRIVATE void __write_char(int column, int line, byte color, byte ch) {
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 0) = ch;
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 1) = color;
}

PRIVATE rpc_args_struct wrapper_write_char(int caller, rpc_func func_no, rpc_args_struct args) {
    int column = ARG_READ(args.data, int);
    int line = ARG_READ(args.data, int);
    byte color = ARG_READ(args.data, byte);
    byte ch = ARG_READ(args.data, byte);

    color &= 0xFF;
    ch &= 0xFF;

    __write_char(column, line, color, ch);

    bool *result = malloc(sizeof(bool));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(bool)};
}

PUBLIC void kmod_main(void) {
    set_logging_name("Video");
    check_ipc();
    recv_msg(&video_info);
    
    register_normal_ipc_handler(normal_ipc_handler);
    rpc_register(0, wrapper_write_char, sizeof(bool), sizeof(int) * 2 + sizeof(byte) * 2);
    set_allow(ANY_TASK);

    message_loop();

    while (true);
}