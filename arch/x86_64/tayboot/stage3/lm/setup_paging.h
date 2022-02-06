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
 * arch/x86_64/tayboot/stage3/lm/setup_paging.h
 *
 * 设置页表
 */



#pragma once

#include <tayboot/tay_defs.h>

#define PAGE_ADDRESS (0x620000)
PUBLIC void *setup_paging(dword memsz, dword memsz_high, void** limit);