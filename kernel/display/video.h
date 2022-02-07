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
 * video.h
 * 
 * 视频
 * 
 */



#pragma once

#include <tayhuang/defs.h>

typedef int color_rgb;
typedef int color_rgba;

#define MAKE_RGB(r, g, b) ((((int)(r)) << 16) | (((int)(g)) << 8) | ((int)(b)))
//#define MAKE_RGBA(r, g, b, a)

enum {
    VGA_COLOR16_BLACK = 0,
    VGA_COLOR16_BLUE = 1,
    VGA_COLOR16_GREEN = 2,
    VGA_COLOR16_CYAN = 3,
    VGA_COLOR16_RED = 4,
    VGA_COLOR16_MAGENTA = 5,
    VGA_COLOR16_BROWN = 6,
    VGA_COLOR16_WHITE = 7,
    VGA_COLOR16_LIGHTING = 8
};

PUBLIC void init_video(void *framebuffer, int width, int height, bool graphic);
PUBLIC void init_graphic_shell(int columns, int lines, int start_x, int start_y, int character_width, int character_height);
PUBLIC void draw_character(int column, int line, char ch, int color);
PUBLIC void draw_pixel(int x, int y, color_rgb color);