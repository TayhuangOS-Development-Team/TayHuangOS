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
 * __sys_task_fn.h
 * 
 * sys_task 函数功能
 * 
 */



#pragma once

#include <tayhuang/defs.h>

//共享内存
#define SHARE_MEMORY_FN (0)
#define SHARE_MEMORY_ARGS_SIZE (sizeof(void*) + sizeof(int) * 2)
#define SHARE_MEMORY_RETURN_TYPE void*

//创建共享内存
#define CREATE_SHARE_MEMORY_FN (1)
#define CREATE_SHARE_MEMORY_ARGS_SIZE (sizeof(int))
#define CREATE_SHARE_MEMORY_RETURN_TYPE void*

//共享内存
PUBLIC void *share_memory(void *addr, int pages, int target);
//创建共享内存
PUBLIC void *create_share_memory(int pages);