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
 * fifo.h
 * 
 * FIFO POOL
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <fifo.h>

#define FIFO_ADDRESS (0x2000000)
#define FIFO_SIZE (0x500000)

PUBLIC void mapping_fifo(void);
PUBLIC void mark_fifo_used(void);
PUBLIC void init_fifo(void);
PUBLIC void *alloc_fifo(int size);