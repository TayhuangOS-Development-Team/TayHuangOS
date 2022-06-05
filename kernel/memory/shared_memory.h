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
 * shared_memory.h
 * 
 * 共享内存
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void *shm_create(int pages, int src_pid);
PUBLIC bool shm_mapping(void *mem, int pages, int src_pid, int target_pid);
PUBLIC void shm_delete(void *addr, int pages, int src_pid);