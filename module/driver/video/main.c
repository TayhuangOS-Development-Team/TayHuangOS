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
#include <tayhuang/video_info.h>

PRIVATE video_info_struct VIDEO_INFO;

PUBLIC void kmod_main(void) {
    set_logging_name("Video");
    linfo ("233");

    check_ipc();
    recv_msg(&VIDEO_INFO);

    *(word*)(VIDEO_INFO.framebuffer + 0) = 0x0C56;
    *(word*)(VIDEO_INFO.framebuffer + 2) = 0x0C49;
    *(word*)(VIDEO_INFO.framebuffer + 4) = 0x0C44;
    *(word*)(VIDEO_INFO.framebuffer + 6) = 0x0C45;
    *(word*)(VIDEO_INFO.framebuffer + 8) = 0x0C4F;

    while (true);
}