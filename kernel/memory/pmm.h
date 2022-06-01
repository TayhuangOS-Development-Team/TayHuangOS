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
 * pmm.h
 *
 * 物理内存管理器
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void init_pmm(qword pmemsz); //初始化PMM
PUBLIC void *lookup_free_page(void); //寻找空闲页
PUBLIC void mark_used(void *page); //标志被使用过
PUBLIC void mark_unused(void *page); //标志未被使用过
PUBLIC void *find_freepages(int max, int *found);
PUBLIC void *find_continue_freepages(int num); //寻找连续的一块内存