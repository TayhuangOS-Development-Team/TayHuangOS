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
 * kmod_prototypes.h
 * 
 * 内核模块原型
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void kmod_init(void);
PUBLIC void kmod_main(void);

#define KMOD_MAGIC (0x71BA4851)