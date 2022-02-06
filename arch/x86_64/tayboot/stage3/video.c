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
 * arch/x86_64/tayboot/stage3/video.c
 *
 * 视频函数
 *
 */



#include "video.h"

PUBLIC struct DP_INFO DPINFO;

PUBLIC void init_video(int screen_width, int screen_height, int type, char *framebuffer) {
    DPINFO.screen_width = screen_width; //设置屏幕宽高
    DPINFO.screen_height = screen_height;
    DPINFO.type = type; //设置类型
    DPINFO.framebuffer = framebuffer; //设置显存
}

PUBLIC void draw_pixel(int pos_x, int pos_y, color_rgb *color) {
    if (DPINFO.type != DPM_GRAPHIC) //不是图形模式则不运行
        return;
    int position = pos_y * DPINFO.screen_width + pos_x;
    *(DPINFO.framebuffer + position * 3) = color->b;
    *(DPINFO.framebuffer + position * 3 + 1) = color->g;
    *(DPINFO.framebuffer + position * 3 + 2) = color->r;
}

PUBLIC void draw_rectangle(int lu_x, int lu_y, int rd_x, int rd_y, color_rgb *color) {
    //依靠循环实现
    for (int i = lu_x ; i < rd_x ; i ++) {
        for (int j = lu_y ; j < rd_y ; j ++) {
            draw_pixel(i, j, color);
        }
    }
}

PUBLIC void draw_character(int pos_x, int pos_y, char ch, byte color) {
    //画字符
    if (DPINFO.type != DPM_CHARACTER)
        return;
    qword position = pos_y * DPINFO.screen_width + pos_x;
    *(DPINFO.framebuffer + position * 2) = ch;
    *(DPINFO.framebuffer + position * 2 + 1) = color;
}