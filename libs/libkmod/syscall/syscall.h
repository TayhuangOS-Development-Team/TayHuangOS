/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * syscall.h
 * 
 * 系统调用
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void moo(void);
PUBLIC void reg_irq(int irq);
PUBLIC bool test_and_lock(bool *val);