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

#include <syscall/ipc.h>
#include <syscall/rpc.h>

#include <global.h>

PUBLIC video_info_struct video_info;

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    set_allow(ANY_TASK);
}

PUBLIC void kmod_init(void) {
    set_logging_name("Video");
    
    check_ipc();
    recv_msg(&video_info);
    
    register_normal_ipc_handler(normal_ipc_handler);
    text_register_rpc_functions();
    set_allow(ANY_TASK);
}

PUBLIC void kmod_main(void) {
}