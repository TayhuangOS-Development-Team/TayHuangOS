// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/keyboard/keyboard_driver.h
 *
 * 键盘驱动
 */



#pragma once

#include "../driver.h"

#define KB_CMD_READ_KEY CMD_TY(0) // word*

PUBLIC void create_keyboard_driver(pdriver driver);