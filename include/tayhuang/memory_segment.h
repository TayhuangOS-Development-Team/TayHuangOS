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
    MST_USING = 1,
    MST_PAGE = 2,
    MST_HARDWARE = 3,
    MST_RESERVED = 4,
    MST_ERROR = 5,
    MST_UNPRESENT = 6,
    MST_SWAP = 8,
    MST_PROTECT = 16,
    MST_DATA = 32
};

typedef struct {
    void *base;
    void *limit;
    int type;
    void *nxt;
} memory_segment;