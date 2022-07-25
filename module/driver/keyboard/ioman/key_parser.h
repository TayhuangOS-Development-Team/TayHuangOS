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
 * key_parser.h
 * 
 * 解析器
 * 
 */



#pragma once

#include <tayhuang/defs.h>

//状态
EXTERN PUBLIC int state;

enum {
    NORMAL = 0,
    E0_XX = 1,
    E1_XX = 2,
    E1_STAGE_UNIT = 4,
    E1_STAGE_MASK = 28,
    E1_STAGE_END = 20,
    E1_FAILED = 32,
    NUMLOCKING = 64,
    CAPSLOCKING = 128,
    LSHIFT_PUSHED = 1024,
    RSHIFT_PUSHED = 2048,
};

PUBLIC short do_normal(byte code);
PUBLIC short do_e0(byte code);
PUBLIC short do_e1(byte code);
PUBLIC short do_shift(byte code);