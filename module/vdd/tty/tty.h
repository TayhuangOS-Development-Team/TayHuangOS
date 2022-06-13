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
 * tty.h
 *
 * tty
 *
 */



#pragma once

#include <tayhuang/types.h>

typedef struct {
    int pos_x;
    int pos_y;

    int scroll_line;
    offset_t offset;
} tty_struct, *tty;