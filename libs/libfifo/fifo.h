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

PUBLIC void *create_fifo(size_t fifo_size);
PUBLIC void *share_fifo(void *fifo, int target);
PUBLIC bool fifo_write_bytes(void *fifo, byte *data, size_t size);
PUBLIC bool fifo_read_bytes(void *fifo, byte *data, size_t size);
PUBLIC size_t fifo_read_all(void *fifo, byte *data);
PUBLIC size_t fifo_get_size(void *fifo);
PUBLIC size_t fifo_get_used_size(void *fifo);
PUBLIC size_t fifo_get_free_size(void *fifo);