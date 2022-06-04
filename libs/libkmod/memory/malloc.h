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

bool init_heap(void);
void *malloc(int size);
void free(void *addr);