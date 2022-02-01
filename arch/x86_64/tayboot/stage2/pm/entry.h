// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/pm/entry.c
 *
 * 保护模式入口
 */



#pragma once

#include "../header.h"

#define STAGE3_BIN_SEGMENT 0x1000 //stage3.bn所在段
#define STAGE3_BIN_OFFSET 0x0000 //stage3.bin的偏移
#define STAGE3_SEGMENT 0x0000 //stage3所在段

PUBLIC void go_to_protect_mode(void); //进入保护模式
PUBLIC void* load_stage3(void); //加载stage3