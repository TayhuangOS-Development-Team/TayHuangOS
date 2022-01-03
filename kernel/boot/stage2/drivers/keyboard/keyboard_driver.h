// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/keyboard/keyboard_driver.h
 *
 * Keyboard driver is declared here
 */



#pragma once

#include "../driver.h"

#define KB_CMD_READ_KEY CMD_TY(0) // word*

PUBLIC void create_keyboard_driver(pdriver driver);