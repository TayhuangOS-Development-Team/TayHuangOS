/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * framebuffers.h
 * 
 * framebuffer表
 * 
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct {
    void *framebuffer;
    int column;
    int line;
    int width;
    int height;
} frame_t;

PUBLIC frame_t *get_framebuffer(int id);
PUBLIC void set_framebuffer(int id, void *framebuffer, int column, int line, int width, int height);
PUBLIC bool has_framebuffer(int id);
PUBLIC void remove_framebuffer(int id);