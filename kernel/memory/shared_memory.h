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

PUBLIC void *shm_create(int pages);
PUBLIC void *shm_share(void *mem, int pages, int pid);
PUBLIC void shm_delete(void *addr, int pages);