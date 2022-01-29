// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/console/console.h
 *
 * Real mode console here
 */



#pragma once

#include "../header.h"

PUBLIC void enter_console(void);
extern PUBLIC char user_name[32];
extern PUBLIC bool logined;