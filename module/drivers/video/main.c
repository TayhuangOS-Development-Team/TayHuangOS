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

int display_mode = DISP_MODE_NONE;

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

void kmod_main(void) {
    linfo ("I'm video!");

    while (recv_any_msg(&display_mode) == -1);

    char buffer[256];

    itoa(display_mode, buffer, 10);

    linfo (buffer);

    while (true) {
        qword commands[64];
        int caller = 0;

        while ((caller = recv_any_msg(commands)) == -1);

        deal_cmd(caller, commands[0], &commands[1]);
    }
    while (true);
}