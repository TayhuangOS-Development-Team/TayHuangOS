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
 * load_kernel.h
 *
 * 加载内核
 *
 */



#pragma once

#include <tayboot/tay_defs.h>

#define DISK_MOD_ADDRESS (0x200000)

void *load_kernel(void** kernel_start, void** kernel_limit);