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
 * memory_segment.h
 *
 * 内存段
 *
 */



#pragma once

enum {
    MST_FREE = 0,
    MST_USING,
    MST_PROTECT,
    MST_PAGE,
    MST_HARDWARE,
    MST_RESERVED,
    MST_ERROR,
    MST_UNPRESENT
};

typedef struct {
    void *base;
    void *limit;
    int type;
    void *nxt;
} memory_segment;