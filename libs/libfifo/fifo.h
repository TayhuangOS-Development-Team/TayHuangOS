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
 * fifo
 *
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct {
	void *buffer;
	offset_t rd_pos;
	offset_t wr_pos;
	int size;
	int len;
} fifo_struct;

void create_fifo(fifo_struct *fifo, void *buffer, int size);
bool fifo_empty(fifo_struct *fifo);
bool fifo_full(fifo_struct *fifo);
bool write_fifo(fifo_struct *fifo, void *data, int size);
bool read_fifo(fifo_struct *fifo, void *data, int len);
int get_fifo_size(fifo_struct *fifo);
int get_fifo_len(fifo_struct *fifo);