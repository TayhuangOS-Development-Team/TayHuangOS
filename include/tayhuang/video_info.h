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
 * video_info_struct.h
 * 
 * 视频信息
 * 
 */



#pragma once

typedef struct {
    void *framebuffer;
    int width;
    int height;
    bool is_graphic_mode;
} video_info_struct;