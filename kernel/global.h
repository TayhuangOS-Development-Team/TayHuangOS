/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * global.h
 * 
 * 全局的信息/变量
 * 
 */



#pragma once

#include <tayhuang/defs.h>

EXTERN PUBLIC void *kernel_pml4;
EXTERN PUBLIC struct boot_args args;
EXTERN PUBLIC int cur_pid;
EXTERN PUBLIC bool entered_handler;

#define tr_idx (1)
#define cs3_idx (3)
#define ds3_idx (4)
#define cs1_idx (5)
#define ds1_idx (6)
#define cs0_idx (7)
#define ds0_idx (8)

#define CS_USER (cs3_idx << 3 | 3)
#define DS_USER (ds3_idx << 3 | 3)
#define RFLAGS_USER ((1 << 9) | (3 << 12))
#define CS_SERVICE (cs1_idx << 3 | 1)
#define DS_SERVICE (ds1_idx << 3 | 1)
#define RFLAGS_SERVICE ((1 << 9) | (1 << 12))
#define CS_KERNEL (cs0_idx << 3)
#define DS_KERNEL (ds0_idx << 3)
#define RFLAGS_KERNEL (1 << 9)

#define RING0_STACKTOP      (0x14C0000)
#define RING0_STACKBOTTOM   (0x1480000)
#define RING0_STACKTOP2     (0x1480000)
#define RING0_STACKBOTTOM2  (0x1440000)
#define RING0_STACKTOP3     (0x1440000)
#define RING0_STACKBOTTOM3  (0x1400000)
#define IST0_STACKTOP       (0x1400000)
#define IST0_STACKBOTTOM    (0x13C0000)

#define DEFAULT_SHM_START   (0x100000000)
#define DEFAULT_SHM_END     (0x200000000)

PUBLIC void mapping_kernel(void *pgd);