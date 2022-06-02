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
 * entry.h
 *
 * 长模式入口
 *
 */



#pragma once

#include <tayboot/tay_defs.h>
#include <tayboot/stage3_args.h>

PUBLIC void goto_longmode(struct stage3_args *args, word selector64);