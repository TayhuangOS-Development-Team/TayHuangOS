/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * setup_longmode.h
 *
 * 设置长模式
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC ASSEMBLY("lm/lm_operators.asm", 106) void goto_ia32e(NONNULL void *entrypoint, NONNULL void *args, word selector64);
PUBLIC ASSEMBLY("lm/lm_operators.asm", 109) void flush_cs(void);