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

#include <types.h>

#define MEMUNIT_SZ (4096)

void *shm_create(int pages);
bool shm_mapping(void *mem, int pages, int pid);
void shm_delete(void *addr, int pages);