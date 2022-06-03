
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
 * malloc.c
 * 
 * malloc函数
 * 
 */



#include "malloc.h"

#include <types.h>
#include <ipc/ipc.h>
#include <debug/logging.h>

#define TASKMAN_PID (0x10002)
#define GET_START_HEAP (1)
#define GET_END_HEAP (2)

void *get_start_heap(int pid) {
    void *start_heap;
    int command[] = {GET_START_HEAP, pid};
    sendrecv(command, &start_heap, TASKMAN_PID, sizeof(command), 20);
    return start_heap;
}

void *get_end_heap(int pid) {
    void *end_heap;
    int command[] = {GET_END_HEAP, pid};
    sendrecv(command, &end_heap, TASKMAN_PID, sizeof(command), 20);
    return end_heap;
}

//一次分配16的倍数个内存
#define HEAPUNIT_SZ (16)
//小于等于该值时不再对区块进行分割
#define HEAPDIV_MIN_SZ (128)

//chunk起始地址 + 16B 即为数据所在地址
typedef struct _chunk {
    qword size : 63;
    bool used : 1;
    struct _chunk *next;
} chunk; //16B
//堆开头16个Byte为空闲chunk头

void theap_init(int pid) {
    void *heap = get_start_heap(pid);

    chunk *whole_heap_chunk = (chunk*)(heap + 2);
    whole_heap_chunk->size = get_end_heap(pid) - get_start_heap(pid) - 2;
    whole_heap_chunk->used = false;
    whole_heap_chunk->next = NULL;

    chunk *free_chunk_head = (chunk*)heap;
    free_chunk_head->size = -1;
    free_chunk_head->used = false;
    free_chunk_head->next = whole_heap_chunk;
}

void *tmalloc(int size, int pid) {
    void *heap = get_start_heap(pid);
    chunk *free_chunk = (chunk*)(heap + 2);
    chunk *last = (chunk*)heap;

    //修正大小
    int fix_size = ((size / HEAPUNIT_SZ) + (size % HEAPUNIT_SZ != 0)) * HEAPUNIT_SZ;

    //寻找符合条件的chunk
    while (free_chunk->size < (fix_size + 2) && (free_chunk->next != NULL)) {
        last = free_chunk;
        free_chunk = free_chunk->next;
    }

    if (free_chunk->size < (fix_size + 2)) {
        //TODO: extend the heap
    }

    //TODO: div the chunk

    //进行分割
    if (free_chunk->size > HEAPDIV_MIN_SZ) {
        chunk *new_chunk = ((void*)free_chunk) + 2 + fix_size;
        new_chunk->next = free_chunk->next;
        new_chunk->size = free_chunk->size - 2 - fix_size;
        new_chunk->used = false;
        last->next = new_chunk;

        free_chunk->size = fix_size + 2;
    }
    else {
        last->next = NULL;
    }

    //更改信息
    free_chunk->used = true;
    free_chunk->next = NULL;

    return ((void*)free_chunk) + 2;
}

void tfree(void *addr, int pid) {
    void *heap = get_start_heap(pid);

    //空闲chunk链表头
    chunk *free_chunk = (chunk*)(heap + 2);

    //当前chunk
    chunk *cur_chunk = (chunk*)(addr - 2);
    if (! cur_chunk->used) {
        lwarn ("try to free a unused chunk!");
        return;
    }

    //设为未使用
    cur_chunk->used = false;
    cur_chunk->next = NULL;

    //寻找链表尾
    while (free_chunk->next != NULL) {
        free_chunk = free_chunk->next;
    }

    free_chunk->next = cur_chunk; //插入
}