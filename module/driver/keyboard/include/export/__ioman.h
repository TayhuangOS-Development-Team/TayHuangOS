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
 * __ioman.h
 * 
 * IO Manager Functions
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define SHARE_KEYBUFFER_FN (0)
#define SHARE_KEYBUFFER_ARGS_SIZE (sizeof(bool))
#define SHARE_KEYBUFFER_RETURN_TYPE void *

PUBLIC void *share_keybuffer(bool flag);