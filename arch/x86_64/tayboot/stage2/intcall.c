/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/intcall.h
 *
 * 实现中断调用函数
 *
 */



#include "intcall.h"

void _intcall(pintargs args);
PUBLIC void intcall(pintargs args) {
    _intcall(args);
    ed_callasm();
}