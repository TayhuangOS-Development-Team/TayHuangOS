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
    int pages = (fifo_size + MEMUNIT_SZ - 1) / MEMUNIT_SZ; //获得页数

    linfo ("Fifo: Create fifo, size=%d(%d pages)", fifo_size, pages);

    //创建
    fifo_struct *fifo = (fifo_struct *)create_share_memory(pages);

    //初始化信息
    fifo->lock = false;
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

PUBLIC bool fifo_write_bytes(void *fifo, byte *data, size_t size) {
    //判断缓冲区有没有空间进行写入
    if (size > fifo_get_free_size(fifo)) {
        return false;
    }

    fifo_struct *_fifo = (fifo_struct *)fifo;
    wait_and_lock(_fifo); //上锁

    if (_fifo->write_offset + size <= _fifo->fifo_size) { //没有跨越边界
        memcpy(&_fifo->buffer[_fifo->write_offset], data, size); //复制

        _fifo->write_offset += size; //移动指针
        _fifo->used_size += size; //增加已使用大小

        if (_fifo->write_offset == _fifo->fifo_size) { //回到开始位置
            _fifo->write_offset = 0;
        }
    }
    else { //跨越边界了
        size_t tail_size = _fifo->fifo_size - _fifo->write_offset; //缓冲区尾部的剩余大小
        size_t head_size = size - tail_size; //缓冲区头部还需要的大小

        memcpy(&_fifo->buffer[_fifo->write_offset], data, tail_size); //复制
        memcpy(&_fifo->buffer[0], data + tail_size, head_size); //复制

        _fifo->write_offset = head_size; //移动指针
        _fifo->used_size += size; //增加已使用大小
    }

    unlock(_fifo); //解锁

    return true;
}

PUBLIC bool fifo_read_bytes(void *fifo, byte *data, size_t size) {
    if (size > fifo_get_used_size(fifo)) {
        return false;
    }

    fifo_struct *_fifo = (fifo_struct *)fifo;
    wait_and_lock(_fifo); //上锁

    if (_fifo->read_offset + size <= _fifo->fifo_size) { //没有跨越边界
        memcpy(data, &_fifo->buffer[_fifo->read_offset], size); //复制

        _fifo->read_offset += size; //移动指针
        _fifo->used_size -= size; //减少已使用大小

        if (_fifo->read_offset == _fifo->fifo_size) { //回到开始位置
            _fifo->read_offset = 0;
        }
    }
    else { //跨越边界了
        size_t tail_size = _fifo->fifo_size - _fifo->read_offset; //缓冲区尾部的剩余大小
        size_t head_size = size - tail_size; //缓冲区头部还需要的大小

        memcpy(data, &_fifo->buffer[_fifo->read_offset], tail_size); //复制
        memcpy(data + tail_size, &_fifo->buffer[0], head_size); //复制

        _fifo->read_offset = head_size; //移动指针
        _fifo->used_size -= size; //减少已使用大小
    }

    unlock(_fifo); //解锁

    return true;
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