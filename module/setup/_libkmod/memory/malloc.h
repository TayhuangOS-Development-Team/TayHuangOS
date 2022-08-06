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
 * malloc.h
 * 
 * 分配内存
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC bool init_heap(int pid, void *heap_start, void *heap_end);
PUBLIC void *malloc(int size);
PUBLIC void free(void *addr);