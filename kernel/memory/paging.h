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

PUBLIC void set_pml4(_IN void *pml4);
PUBLIC void *get_pml4(void);
PUBLIC void *init_paging(void);
PUBLIC bool set_mapping(_IN void *from, _IN void *to, _IN int pages, _IN bool rw, _IN bool us);