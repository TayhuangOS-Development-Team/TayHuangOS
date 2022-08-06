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
 * fifo.h
 * 
 * First In First Out Buffer
 * 
 */



#pragma once

#include <tayhuang/defs.h>

// 创建出的缓冲区大小一般不等于此值
// 创建出的缓冲区大小为离fifo_size最近的MEMUNIT_SZ的倍数减去缓冲区信息的大小
PUBLIC void *create_fifo(size_t fifo_size);
//共享FIFO
PUBLIC void *share_fifo(void *fifo, int target);
//向FIFO写
PUBLIC void fifo_write_bytes(void *fifo, byte *data, size_t size);
//从FIFO读
PUBLIC void fifo_read_bytes(void *fifo, byte *data, size_t size);
PUBLIC size_t fifo_read_all(void *fifo, byte *data);
//FIFO信息
PUBLIC size_t fifo_get_size(void *fifo);
PUBLIC size_t fifo_get_used_size(void *fifo);
PUBLIC size_t fifo_get_free_size(void *fifo);