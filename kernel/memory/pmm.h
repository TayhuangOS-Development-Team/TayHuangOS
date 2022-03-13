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

PUBLIC void init_pmm(qword pmemsz);
PUBLIC void *lookup_free_page(void);
PUBLIC void mark_used(void *page);
PUBLIC void mark_unused(void *page);