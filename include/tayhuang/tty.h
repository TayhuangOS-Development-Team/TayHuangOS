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
    void *teminal_buffer;
    device_t video;
    device_t keyboard;
} tty_struct, *tty;