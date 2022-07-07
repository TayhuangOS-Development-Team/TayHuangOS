/*
 * SPDX-License-Identifier: GPL-3.0-only
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

PUBLIC void *create_pgd(void); //创建页表
PUBLIC bool set_mapping(void *pgd, void *vaddr, void *paddr, int pages, bool rw, bool us); //设置映射
PUBLIC void *get_physical_address(void *__pgd, void *vaddr); //获取物理地址

#define __pa(pgd, vaddr) (get_physical_address(pgd, vaddr))