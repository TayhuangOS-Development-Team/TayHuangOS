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
#include <string.h>

typedef struct {
    bool lock;
    size_t write_offset;
    size_t read_offset;
    size_t fifo_size;
    size_t used_size;
    char buffer[0];
} fifo_struct;

PRIVATE void wait_and_lock(fifo_struct *fifo) {
    while (! test_and_lock(&fifo->lock)); //没上锁则一直进行检测并上锁操作
}

PRIVATE void unlock(fifo_struct *fifo) {
    fifo->lock = false;
}

PUBLIC void *create_fifo(size_t fifo_size) {
    int pages = (fifo_size + MEMUNIT_SZ - 1) / MEMUNIT_SZ;

    linfo ("Fifo: Create fifo, size=%d(%d pages)", fifo_size, pages);

    fifo_struct *fifo = (fifo_struct *)create_share_memory(pages);
    fifo->lock = false;
    fifo->fifo_size = pages * MEMUNIT_SZ - sizeof(fifo_struct);
    fifo->write_offset = 0;
    fifo->read_offset = 0;
    fifo->used_size = 0;

    return fifo;
}

PUBLIC void *share_fifo(void *fifo, int target) {
    fifo_struct *_fifo = (fifo_struct *)fifo;
    int pages = (_fifo->fifo_size + sizeof(fifo_struct)) / MEMUNIT_SZ;

    void *new_fifo = share_memory(fifo, pages, target);

    return new_fifo;
}

PUBLIC bool fifo_write_bytes(void *fifo, byte *data, size_t size) {
    if (size > fifo_get_free_size(fifo)) {
        return false;
    }

    fifo_struct *_fifo = (fifo_struct *)fifo;
    wait_and_lock(_fifo);

    if (_fifo->write_offset + size <= _fifo->fifo_size) {
        memcpy(&_fifo->buffer[_fifo->write_offset], data, size);

        _fifo->write_offset += size;
        _fifo->used_size += size;

        if (_fifo->write_offset == _fifo->fifo_size) {
            _fifo->write_offset = 0;
        }
    }
    else {
        size_t tail_size = _fifo->fifo_size - _fifo->write_offset;
        size_t head_size = size - tail_size;

        memcpy(&_fifo->buffer[_fifo->write_offset], data, tail_size);
        memcpy(&_fifo->buffer[0], data + tail_size, head_size);

        _fifo->write_offset = head_size;
        _fifo->used_size += size;
    }

    unlock(_fifo);

    return true;
}

PUBLIC bool fifo_read_bytes(void *fifo, byte *data, size_t size) {
    if (size > fifo_get_used_size(fifo)) {
        return false;
    }

    fifo_struct *_fifo = (fifo_struct *)fifo;
    wait_and_lock(_fifo);

    if (_fifo->read_offset + size <= _fifo->fifo_size) {
        memcpy(data, &_fifo->buffer[_fifo->read_offset], size);

        _fifo->read_offset += size;
        _fifo->used_size -= size;

        if (_fifo->read_offset == _fifo->fifo_size) {
            _fifo->read_offset = 0;
        }
    }
    else {
        size_t tail_size = _fifo->fifo_size - _fifo->read_offset;
        size_t head_size = size - tail_size;

        memcpy(data, &_fifo->buffer[_fifo->read_offset], tail_size);
        memcpy(data + tail_size, &_fifo->buffer[0], head_size);

        _fifo->read_offset = head_size;
        _fifo->used_size -= size;
    }

    unlock(_fifo);

    return true;
}

PUBLIC size_t fifo_read_all(void *fifo, byte *data) {
    size_t size = ((fifo_struct *)fifo)->used_size;
    fifo_read_bytes(fifo, data, size);
    return size;
}

PUBLIC size_t fifo_get_size(void *fifo) {
    return ((fifo_struct *)fifo)->fifo_size;
}

PUBLIC size_t fifo_get_used_size(void *fifo) {
    return ((fifo_struct *)fifo)->used_size;
}

PUBLIC size_t fifo_get_free_size(void *fifo) {
    return fifo_get_size(fifo) - fifo_get_used_size(fifo);
}