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
 * paging.h
 *
 * 页表
 *
 */



#pragma once

#include <tayhuang/defs.h>

//创建页表
PUBLIC void *create_pgd(void); 
//设置映射
PUBLIC bool set_mapping(void *pgd, void *vaddr, void *paddr, int pages, bool rw, bool us); 
//获取物理地址
PUBLIC void *get_physical_address(void *__pgd, void *vaddr); 

#define __pa(pgd, vaddr) (get_physical_address(pgd, vaddr))