// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage3/vedio.h
 *
 * Vedio functions are declared here
 */



#pragma once

#include <tay_defs.h>

typedef struct {
    byte r;
    byte g;
    byte b;
    byte reserved;
} color_rgb;

typedef struct {
    byte r;
    byte g;
    byte b;
    byte a;
} color_rgba;

enum {
    DPM_CHARACTER = 0,
    DPM_GRAPHIC
};

struct DP_INFO {
    int screen_width;
    int screen_height;
    int type;
    char* framebuffer;
};

extern struct DP_INFO DPINFO;

PUBLIC void init_vedio(int screen_width, int screen_height, int type, char* framebuffer);
PUBLIC void draw_pixel(int pos_x, int pos_y, color_rgb color);
PUBLIC void draw_character(int pos_x, int pos_y, char ch, byte color);