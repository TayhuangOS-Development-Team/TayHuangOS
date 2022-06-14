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
 * modes.h
 *
 * 模式
 *
 */



#pragma once

enum {
    DISP_MODE_NONE = 0,
    DISP_MODE_TEXT,
    DISP_MODE_GRAPHIC
};

enum {
    MODE_ANY = 0,
    MODE_TEXT = 1,
    MODE_GRAPHIC = 2
};

#define MODE(cmd) (((cmd) >> 28) & 0xF)
#define CODE(cmd) ((cmd) & 0x0FFFFFFF)

#define MKCMD(mode, code) ((((qword)(mode & 0xF)) << 28) | (((qword)(code)) & 0x0FFFFFFF))