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
 * __video_driver_fn.h
 * 
 * video_driver 函数功能
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define TEXT_WRITE_CHAR_FN (0)
#define TEXT_WRITE_CHAR_ARGS_SIZE (sizeof(int) * 2 + sizeof(byte) * 2)
#define TEXT_WRITE_CHAR_RETURN_TYPE bool

#define TEXT_WRITE_STRING_FN (1)
#define TEXT_WRITE_STRING_ARGS_SIZE (-1)
#define TEXT_WRITE_STRING_RETURN_TYPE bool

#define CREATE_FRAMEBUFFER_FN (2)
#define CREATE_FRAMEBUFFER_ARGS_SIZE (sizeof(int) * 2)
#define CREATE_FRAMEBUFFER_RETURN_TYPE void*

#define SWAP_FRAMEBUFFER_FN (3)
#define SWAP_FRAMEBUFFE_ARGS_SIZE (0)
#define SWAP_FRAMEBUFFE_RETURN_TYPE bool