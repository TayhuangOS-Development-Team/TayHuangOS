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
 * malloc.h
 * 
 * malloc函数
 * 
 */



#pragma once

#include <types.h>

bool theap_init(int pid);
void *tmalloc(int size, int pid);
void tfree(void *addr, int pid);