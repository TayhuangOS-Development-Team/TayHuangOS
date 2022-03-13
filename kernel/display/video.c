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
 * video.c
 *
 * 视频
 *
 */



#include "video.h"

PUBLIC struct VIDEO_INFO_STRUCT VIDEO_INFO;

PUBLIC void init_video(void *framebuffer, int width, int height, bool graphic) {
    VIDEO_INFO.framebuffer = framebuffer;
    VIDEO_INFO.width = width;
    VIDEO_INFO.height = height;
    VIDEO_INFO.graphic = graphic;
    if (! graphic) {
        VIDEO_INFO.columns = width;
        VIDEO_INFO.lines = height;
    }
}

PRIVATE void load_font(int width, int height) {
    VIDEO_INFO.character_width = width;
    VIDEO_INFO.character_height = height;
    //TODO
}

PUBLIC void init_graphic_shell(int columns, int lines, int start_x, int start_y, int character_width, int character_height) {
    load_font(character_width, character_height);
    VIDEO_INFO.columns = columns;
    VIDEO_INFO.lines = lines;
    VIDEO_INFO.shell_start_x = start_x;
    VIDEO_INFO.shell_start_y = start_y;
}

PRIVATE void draw_character_vga(int column, int line, char ch, byte color) {
    *(((word*)VIDEO_INFO.framebuffer) + line * VIDEO_INFO.columns + column) = MKWORD(color, ch);
}

PRIVATE void draw_character_graphic(int column, int line, char ch, color_rgb color) {
    //TODO
}

PUBLIC void draw_character(int column, int line, char ch, int color) {
    if (VIDEO_INFO.graphic) {
        draw_character_graphic(column, line, ch, color);
    }
    else {
        draw_character_vga(column, line, ch, color);
    }
}

PUBLIC void draw_pixel(int x, int y, color_rgb color) {
    byte save = *(char*)(VIDEO_INFO.framebuffer + (y * VIDEO_INFO.width + x + 1) * 3);
    *(dword*)(VIDEO_INFO.framebuffer + (y * VIDEO_INFO.width + x) * 3) = color;
    *(char*)(VIDEO_INFO.framebuffer + (y * VIDEO_INFO.width + x + 1) * 3) = save;
}