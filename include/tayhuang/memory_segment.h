// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * include/tayhuang/memory_segment.h
 *
 * 内存段
 */



#pragma once

enum {
    MST_FREE = 0,
    MST_PAGE,
    MST_KERNEL,
    MST_RESERVED,
    MST_BUSY,
    MST_ERROR
};

typedef struct {
    void* base;
    int limit;
    int type;
} memory_segment;