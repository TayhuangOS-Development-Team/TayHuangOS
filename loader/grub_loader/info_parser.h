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
 * info_parser.h
 * 
 * 解析multiboot2参数
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <libs/multiboot2.h>

typedef struct {
    qword memsz;
    bool is_graphic;
    int screen_width;
    int screen_height;
    void *framebuffer;
} parse_result_struct;

PUBLIC void parse_args(NONNULL struct multiboot_tag *tag, NONNULL parse_result_struct *result);