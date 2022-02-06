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
 * segment.h
 * 
 * 内存段
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/memory_segment.h>

typedef int SEGMENT_TOKEN;

PUBLIC void init_gdt(void); //初始化GDT
PUBLIC SEGMENT_TOKEN add_segment(_IN void *base, _IN void *limit, _IN int type); //添加段
PUBLIC void delete_segment(_IN SEGMENT_TOKEN token); //删除段
PUBLIC void query_segment(_IN SEGMENT_TOKEN token, _OUT void **base, _OUT void **limit, _OUT int *type); //查询段
PUBLIC int query_memory_state(_IN void *start, _IN void *limit); //查询某一区间内存的状态
                                                                 //若该区间跨越了多个不同的段
                                                                 //则返回MST_ERROR
PUBLIC void *find_free_memory(_IN int size); //寻找空闲内存