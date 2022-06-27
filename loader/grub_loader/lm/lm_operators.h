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
 * setup_longmode.h
 *
 * 设置长模式
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void goto_ia32e(void *entrypoint, void *args, word selector64);