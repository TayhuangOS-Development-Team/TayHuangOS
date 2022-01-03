// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/tools.h
 *
 * Tool functions are declared here
 */



#pragma once

#include "header.h"

PUBLIC dword get_clock_time(void);
PUBLIC byte get_time(void);
PUBLIC int random(dword seed, int min, int max);