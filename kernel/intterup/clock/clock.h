/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * clock.h
 * 
 * 时钟驱动器
 * 
 */



#pragma once

#include <tayhuang/defs.h>

extern volatile int ticks;
PUBLIC short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler);