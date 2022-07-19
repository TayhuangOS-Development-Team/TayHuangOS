/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * shared_memory.h
 * 
 * 共享内存
 * 
 */



#pragma once

#include <tayhuang/defs.h>

//映射内存
//将src进程从mem开始的pages个页映射到target进程
PUBLIC bool shm_mapping(void *mem, int pages, int src_pid, int target_pid);