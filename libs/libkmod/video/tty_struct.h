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
 * tty_struct.h
 *
 * tty结构
 *
 */



#pragma once

#include <video/tty.h>

typedef struct {
    int tty_no;

    void *video_buffer;
    void *keyboard_buffer;
} tty_struct;

static inline void *to_tty(tty_struct *tty) {
    return (void*)tty;
}