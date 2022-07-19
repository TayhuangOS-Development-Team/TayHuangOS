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
 * sys_task.h
 * 
 * 系统调用进程
 * 
 */



#pragma once

#include <tayhuang/defs.h>

//共享内存
#define SHARE_MEMORY_FN (0)
//创建共享内存
#define CREATE_SHARE_MEMORY_FN (1)

//系统调用进程
PUBLIC void sys_task(void);