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
 * 视频驱动主程序
 *
 */



#include <debug/logging.h>
#include <text/cmd.h>
#include <graphic/cmd.h>
#include <modes.h>

#include <ipc/ipc.h>
#include <tool/tostring.h>

#include <memory/malloc.h>
#include <printf.h>

int display_mode;
int screen_width;
int screen_height;
void *VIDEO_MEMORY;

void deal_cmd(int caller, int cmd, qword *param) {
    if (display_mode == DISP_MODE_NONE) {
        lerror ("Wrong display mode!");
    }
    else if (display_mode == DISP_MODE_TEXT) {
        if (MODE(cmd) == MODE_ANY || MODE(cmd) == MODE_TEXT) {
            deal_text_cmd(caller, CODE(cmd), param);
        }
        else if (MODE(cmd) == MODE_GRAPHIC) {
            lerror ("Couldn't deal graphic command in text mode!");
        }
    }
    else if (display_mode == DISP_MODE_GRAPHIC) {
        if (MODE(cmd) == MODE_ANY || MODE(cmd) == MODE_GRAPHIC) {
            deal_graphic_cmd(caller, CODE(cmd), param);
        }
        else if (MODE(cmd) == MODE_TEXT) {
            lerror ("Couldn't deal text command in graphic mode!");
        }
    }
}

void kmod_main(void) {
    set_logging_name("Video");

    qword infomations[16] = {DISP_MODE_NONE, 0, 0, NULL};

    recv_any_msg_and_wait(infomations);

    display_mode = infomations[0];
    screen_width = infomations[1];
    screen_height = infomations[2];
    VIDEO_MEMORY = infomations[3];

    init_heap();

    qword * command = malloc(600 * sizeof(qword));

    while (true) {
        int caller = recv_any_msg_and_wait(command);
        
        deal_cmd(caller, command[0], &command[1]);
    }

    while (true);
}