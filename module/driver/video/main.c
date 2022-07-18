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

#include <memory/malloc.h>

#include <tayhuang/services.h>

#include <global.h>

PUBLIC video_info_struct video_info;

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    set_allow(ANY_TASK);
}

PRIVATE void __write_char(int column, int line, byte color, byte ch) {
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 0) = ch;
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 1) = color;
}


PUBLIC void kmod_main(void) {
    set_logging_name("Video");
    check_ipc();
    recv_msg(&video_info);
    
    register_normal_ipc_handler(normal_ipc_handler);
    text_register_rpc_functions();
    set_allow(ANY_TASK);

    message_loop();

    while (true);
}