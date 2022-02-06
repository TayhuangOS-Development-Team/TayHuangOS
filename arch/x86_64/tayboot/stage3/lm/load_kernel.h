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
 * arch/x86_64/tayboot/stage3/lm/load_kernel.h
 *
 * 加载内核
 *
 */



#pragma once

#include <tayboot/tay_defs.h>

void *load_kernel(void** kernel_start, void** kernel_limit);