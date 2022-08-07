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

//相关信息
typedef struct {
    id_t mutex;

    id_t empty; //还有多少剩余空间
    id_t full; //还有多少内容
    

    size_t write_offset;
    size_t read_offset;
    size_t fifo_size;
    size_t used_size;
    char buffer[0];
} fifo_struct;

PUBLIC void *create_fifo(size_t fifo_size) {
    int pages = (fifo_size + MEMUNIT_SZ - 1) / MEMUNIT_SZ; //获得页数

    //创建
    fifo_struct *fifo = (fifo_struct *)create_share_memory(pages);

    //初始化信息
    fifo->mutex = create_signal(1, 1, false);
    fifo->empty = create_signal(fifo_size, fifo_size, false);
    fifo->full = create_signal(fifo_size, 0, false);

    fifo->fifo_size = pages * MEMUNIT_SZ - sizeof(fifo_struct);
    fifo->write_offset = 0;
    fifo->read_offset = 0;
    fifo->used_size = 0;

    return fifo;
}

PUBLIC void *share_fifo(void *fifo, int target) {
    fifo_struct *_fifo = (fifo_struct *)fifo;
    int pages = (_fifo->fifo_size + sizeof(fifo_struct)) / MEMUNIT_SZ; //获得页数

    //进行共享
    void *new_fifo = share_memory(fifo, pages, target);

    return new_fifo;
}

PRIVATE byte fifo_read_byte(fifo_struct *fifo) {
    byte data;

    down_signal(fifo->full); //内容 - 1
    down_signal(fifo->mutex); //进入临界区

    data = fifo->buffer[fifo->read_offset];
    fifo->read_offset ++;
    if (fifo->read_offset == fifo->fifo_size) {
        fifo->read_offset = 0;
    }

    up_signal(fifo->mutex); //离开临界区
    up_signal(fifo->empty); //空位 + 1

    return data;
}

PRIVATE void fifo_write_byte(fifo_struct *fifo, byte data) {
    down_signal(fifo->empty); //空位 - 1
    down_signal(fifo->mutex); //进入临界区

    fifo->buffer[fifo->write_offset] = data;
    fifo->write_offset ++;
    if (fifo->write_offset == fifo->fifo_size) {
        fifo->write_offset = 0;
    }

    up_signal(fifo->mutex); //离开临界区
    up_signal(fifo->full); //内容 + 1
}

PUBLIC void fifo_write_bytes(void *fifo, byte *data, size_t size) {
    fifo_struct *_fifo = (fifo_struct *)fifo;
    
    for (int i = 0 ; i < size ; i ++) {
        fifo_write_byte(_fifo, *(data + i));
    }
}

PUBLIC void fifo_read_bytes(void *fifo, byte *data, size_t size) {
    fifo_struct *_fifo = (fifo_struct *)fifo;
    
    for (int i = 0 ; i < size ; i ++) {
        *(data + i) = fifo_read_byte(_fifo);
    }
}

PUBLIC size_t fifo_read_all(void *fifo, byte *data) {
    size_t size = ((fifo_struct *)fifo)->used_size; //获得已使用大小
    fifo_read_bytes(fifo, data, size); //全部读取
    return size;
}

PUBLIC size_t fifo_get_size(void *fifo) {
    return ((fifo_struct *)fifo)->fifo_size;
}

PUBLIC size_t fifo_get_used_size(void *fifo) {
    return ((fifo_struct *)fifo)->used_size;
}

PUBLIC size_t fifo_get_free_size(void *fifo) {
    return fifo_get_size(fifo) - fifo_get_used_size(fifo); //FIFO大小 - 已使用大小 = 空闲大小
}