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

PUBLIC void *init_paging(_IN qword _vmemsz, _IN qword paging1_start, _OUT qword *pagingsz);
PUBLIC void *_init_paging(_IN qword paging1_start, _IN int pml4_num, _IN int pdpt_num, _IN int pd_num, _IN int pt_num, _OUT qword *pagingsz);
PUBLIC bool set_mapping(_IN void *from, _IN void *to, _IN int pages, _IN bool rw, _IN bool us);