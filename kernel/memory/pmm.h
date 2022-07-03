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
 * 物理内存管理
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/paging.h>

//0~reserved_limit = used
//初始化pmm
PUBLIC void init_pmm(qword memsize, void *reserved_limit);
//分配空闲内存
PUBLIC void *__alloc_free_pages(int order, int *order_give);
PUBLIC void *alloc_pages(int order);
//归还内存
PUBLIC void __return_pages(void *addr, int order);
PUBLIC void return_pages(void *addr, int pages);