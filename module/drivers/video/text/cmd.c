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
 * cmd.c
 *
 * 命令处理器
 *
 */



#include "cmd.h"

#include <debug/logging.h>

#include <infomations.h>

#include <ipc/ipc.h>

void write_char(offset_t offset, int ch, int color, int x, int y) {
    *(char*)(VIDEO_MEMORY + offset + (y * screen_width + x) * 2) = ch;
    *(char*)(VIDEO_MEMORY + offset + 1 + (y * screen_width + x) * 2) = color;
}

void deal_text_cmd(int caller, int cmd, qword *param) {
    switch (cmd)
    {
    case __TEXT_WRITE_CHAR: {
        offset_t offset = param[0];
        int ch = param[1];
        int color = param[2];
        int x = param[3];
        int y = param[4];
        write_char(offset, ch, color, x, y);
        bool status = false;
        send_msg(&status, caller, sizeof(bool), 20);
        break;
    }
    case __TEXT_WRITE_CHARS: {
        int offset = param[1];
        for (int i = 0 ; i < param[0] ; i ++) {
            int ch = param[i * 4 + 2];
            int color = param[i * 4 + 3];
            int x = param[i * 4 + 4];
            int y = param[i * 4 + 5];
            write_char(offset, ch, color, x, y);
        }
        bool status = false;
        send_msg(&status, caller, sizeof(bool), 20);
        break;
    }
    default: {
        lwarn ("VIDEO DRIVER Recieved a unknown command in text mode!");
        break;
    }
    }
}