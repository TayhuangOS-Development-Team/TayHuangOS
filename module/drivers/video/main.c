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
            dispatch_txt_cmd(caller, CODE(cmd), param);
        }
    }
}

qword commands[514];

void kmod_main(void) {
    set_logging_name("VIDEO");

    qword infomations[16] = {DISP_MODE_NONE, 0, 0, NULL};

    while (recv_any_msg(infomations) == -1);

    display_mode = infomations[0];
    screen_width = infomations[1];
    screen_height = infomations[2];
    VIDEO_MEMORY = infomations[3];

    //FIXME: Can't run normally
    //init_heap();

    //qword *commands = malloc(512 * sizeof(qword));

    while (true) {
        int caller = 0;

        while ((caller = recv_any_msg(commands)) == -1);

        deal_cmd(caller, commands[0], &commands[1]);
    }

    free(commands);
    while (true);
}