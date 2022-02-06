/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * video.h
 *
 * 视频函数
 *
 */



#pragma once

#include <tayboot/tay_defs.h>

typedef struct {
    byte r;
    byte g;
    byte b;
    byte reserved;
} color_rgb; //RGB颜色

typedef struct {
    byte r;
    byte g;
    byte b;
    byte a;
} color_rgba; //RGBA颜色

enum {
    DPM_CHARACTER = 0, //字符模式
    DPM_GRAPHIC        //图形模式
};

struct DP_INFO {
    int screen_width;
    int screen_height;
    int type;
    char *framebuffer;
}; //显示信息

extern struct DP_INFO DPINFO; //显示信息

PUBLIC void init_video(int screen_width, int screen_height, int type, char *framebuffer); //初始化视频
PUBLIC void draw_pixel(int pos_x, int pos_y, color_rgb *color); //画像素点
PUBLIC void draw_rectangle(int lu_x, int lu_y, int rd_x, int rd_y, color_rgb *color); //画矩形
PUBLIC void draw_character(int pos_x, int pos_y, char ch, byte color); //画字符