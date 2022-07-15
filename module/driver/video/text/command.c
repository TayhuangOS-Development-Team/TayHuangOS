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
 * command.c
 * 
 * 命令
 * 
 */



#include <global.h>
#include <debug/logging.h>

void write_char(int column, int line, byte color, byte ch) {
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 0) = ch;
    *(byte*)(video_info.framebuffer + ((line * video_info.width) + column) * 2 + 1) = color;
}

void text_execute(int cmdid, void *args) {
    switch (cmdid)
    {
    case TEXT_WRITE_CHAR: {
        int column = ARG_READ(args, int);
        int line = ARG_READ(args, int);
        byte color = ARG_READ(args, byte);
        byte ch = ARG_READ(args, byte);

        color &= 0xFF;
        linfo ("%d %d %d %c", column, line, color, ch);

        write_char(column, line, color, ch);
        break;
    }
    default: {
        lerror ("Unknown Command!");
        break;
    }
    }
}