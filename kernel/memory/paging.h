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
 * paging.h
 *
 * 页表
 *
 */



#pragma once

#include <tayhuang/defs.h>

extern PUBLIC void *kernel_pml4;

PUBLIC void set_pml4(void *pml4);
PUBLIC void *get_pml4(void);
PUBLIC void *create_pgd(void);
PUBLIC bool set_mapping(void *from, void *to, int pages, bool rw, bool us);
PUBLIC void *get_physical_address(void *__pml4, void *vaddr);

#define __pa(p, v) (get_physical_address(p, v))