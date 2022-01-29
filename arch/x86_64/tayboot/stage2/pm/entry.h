// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/pm/entry.c
 *
 * Protect Mode Entry here
 */



#pragma once

#include "../header.h"

#define STAGE3_BIN_SEGMENT 0x1000
#define STAGE3_BIN_OFFSET 0x0000
#define STAGE3_SEGMENT 0x0000

PUBLIC void go_to_protect_mode(void);
PUBLIC void* load_stage3(void);