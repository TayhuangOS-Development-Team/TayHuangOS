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
 * fifo.c
 * 
 * First In First Out Buffer
 * 
 */



#include <fifo.h>

#include <debug/logging.h>
#include <memory/sharemem.h>

#include <syscall/syscall.h>

typedef struct {
    bool lock;
    size_t write_offset;
    size_t read_offset;
    size_t fifo_size;
    size_t used_length;
    char buffer[0];
} fifo_struct;

PRIVATE void wait_and_lock(fifo_struct *fifo) {
    while (! test_and_lock(&fifo->lock)); //没上锁则一直进行检测并上锁操作
}

PUBLIC void *create_fifo(size_t fifo_size) {
    int pages = (fifo_size + MEMUNIT_SZ - 1) / MEMUNIT_SZ;

    linfo ("Fifo: Create fifo, size=%d(%d pages)", fifo_size, pages);

    fifo_struct *fifo = create_share_memory(pages);
    fifo->lock = false;
    fifo->write_offset = 0;
    fifo->read_offset = 0;
    fifo->used_length = 0;

    return fifo;
}