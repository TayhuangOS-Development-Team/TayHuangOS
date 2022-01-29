// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/header.h
 *
 * Global header here
 */



#pragma once

#include <tay_defs.h>

#define BASE_OF_DATA 0x1000
typedef bool(*terminater_t)(void);
PUBLIC bool register_terminater(terminater_t terminater);
PUBLIC bool do_terminate(void);