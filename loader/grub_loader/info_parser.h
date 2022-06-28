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
 * info_parser.h
 * 
 * 解析multiboot2参数
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <libs/multiboot2.h>

typedef struct {
    dword memsz;
    dword memsz_high;
    bool is_graphic;
    int screen_width;
    int screen_height;
    void *framebuffer;
} parse_result_struct;

PUBLIC void parse_args(struct multiboot_tag *tag, parse_result_struct *result);