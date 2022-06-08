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

#include <tayhuang/types.h>

#define MEMUNIT_SZ (4096)

bool shm_mapping(void *mem, int pages, int pid);
void *shm_share(void *src_addr, int pages, int src_pid);
void shm_unshare(void *target_addr);