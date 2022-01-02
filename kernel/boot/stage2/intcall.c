// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/intcall.h
 *
 * Int call function is implemented here
 */



#include "intcall.h"

void _intcall(pintargs args);
PUBLIC void intcall(pintargs args) {
    _intcall(args);
    ed_callasm();
}