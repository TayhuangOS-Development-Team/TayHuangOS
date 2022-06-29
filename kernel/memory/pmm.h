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

//等待修改 改为Buddy算法

PUBLIC void init_pmm(qword totalmem);

PUBLIC qword get_freemem_sum(void);
PUBLIC qword get_usedmem_sum(void);
PUBLIC qword get_totalmem_sum(void);
//设置从addr开始的pages个page状态
PUBLIC void __set_pages_state(void *addr, qword pages, bool used);
//从addr开始的pages个page是否全部空闲
PUBLIC bool __is_pages_all_free(void *addr, qword pages);
//获取pages个连续页（强制性）
PUBLIC void *__find_continuous_pages(qword pages);