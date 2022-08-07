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
 * __keyboard_driver_fn.h
 * 
 * keyboard_driver 函数功能
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <export/keyboard/key_types.h>

#define GETKEY_FN (0)
#define GETKEY_ARGS_SIZE (sizeof(bool))
#define GETKEY_RETURN_TYPE key_t

PUBLIC key_t getkey(bool peek);