// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/pm/a20.h
 *
 * A20 functions are declared here
 */



#pragma once

#include "../header.h"

PUBLIC bool test_a20(int loop_time);
PUBLIC bool test_a20_fast(void);
PUBLIC bool test_a20_normal(void);
PUBLIC bool test_a20_slow(void);
PUBLIC bool test_a20_long(void);
PUBLIC bool empty_8042(void);
PUBLIC void enable_a20_by_bios(void);
PUBLIC void enable_a20_by_8042(void);
PUBLIC void enable_a20_by_portA(void);
PUBLIC bool enable_a20(void);