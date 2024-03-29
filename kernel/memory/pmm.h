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
PUBLIC void init_pmm(size_t memsize, void *reserved_limit);
//分配空闲内存
PUBLIC void *__alloc_free_pages(int order, int *order_give);
PUBLIC void *alloc_pages(int order);
//归还内存
PUBLIC void __return_pages(void *addr, int order);
PUBLIC void return_pages(void *addr, int pages);
//给给定页表分配逻辑上连续的内存
PUBLIC void alloc_vpages(void *pgd, void *addr, int pages);
//虚拟地址memset
PUBLIC void vmemset(void *pgd, void *addr, int val, size_t size);
//虚拟地址拷贝到物理地址
PUBLIC void vpmemcpy(void *dst, void *src_pgd, void *src, size_t size);
//物理地址拷贝到虚拟地址
PUBLIC void pvmemcpy(void *dst_pgd, void *dst, void *src, size_t size);
//虚拟地址拷贝到虚拟地址
PUBLIC void vvmemcpy(void *dst_pgd, void *dst, void *src_pgd, void *src, size_t size);