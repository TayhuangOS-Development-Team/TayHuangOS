
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

#include <ipc/ipc.h>
#include <debug/logging.h>
#include <tayhuang/services.h>
#include <memory/shared_memory.h>

#include <tayhuang/paging.h>

#include <printf.h>

#define GET_CURRENT_PID (0)
#define GET_START_HEAP (1)
#define GET_END_HEAP (2)
#define GET_TASK_START (3)
#define GET_TASK_END (4)
#define SHM_MAPPING (5)
#define GET_TASK_PGD (6)
#define SET_TASK_PGD (7)
#define GET_TASK_START_WITHOUT_STACK (9)
#define GET_TASK_END_WITHOUT_HEAP (10)

void *get_start_heap(int pid) {
    void *start_heap = NULL;
    qword command[] = {GET_START_HEAP, pid};
    sendrecv(command, &start_heap, TASKMAN_SERVICE, sizeof(command), 20);
    return start_heap;
}

void *get_end_heap(int pid) {
    void *end_heap = NULL;
    qword command[] = {GET_END_HEAP, pid};
    sendrecv(command, &end_heap, TASKMAN_SERVICE, sizeof(command), 20);
    return end_heap;
}

void *get_task_start(int pid) {
    void *task_start = NULL;
    qword command[] = {GET_TASK_START, pid};
    sendrecv(command, &task_start, TASKMAN_SERVICE, sizeof(command), 20);

    return task_start;
}

void *get_task_end_without_heap(int pid) {
    void *task_end = NULL;
    qword command[] = {GET_TASK_END_WITHOUT_HEAP, pid};
    sendrecv(command, &task_end, TASKMAN_SERVICE, sizeof(command), 20);
    return task_end;
}

void *get_task_pgd(int pid) {
    void *pgd = NULL;
    qword command[] = {GET_TASK_PGD, pid};
    sendrecv(command, &pgd, TASKMAN_SERVICE, sizeof(command), 20);
    return pgd;
}

bool set_task_pgd(int pid, void *pgd) {
    bool status = false;
    qword command[] = {SET_TASK_PGD, pid, pgd};
    sendrecv (command, &status, TASKMAN_SERVICE, sizeof(command), 20);
    return status;
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
} chunk_struct; //16B
//堆开头16个Byte为空闲chunk头

bool theap_init(int pid) {
    char buffer[256];
    sprintf (buffer, "Initialize Heap for PID %d", pid);
    linfo (buffer);

    int cur_pid = get_current_pid();

    void *mm_start = get_task_start(cur_pid);
    void *mm_end = get_task_end_without_heap(cur_pid);
    sprintf (buffer, "MM Start %P End %P", mm_start, mm_end);
    linfo (buffer);
    
    int len = mm_end - mm_start;
    int pages = (len / MEMUNIT_SZ) + ((len % MEMUNIT_SZ) != 0);
    shm_mapping(mm_start, pages, pid);

    void *heap = get_start_heap(pid);

    void *origin_pgd = get_task_pgd(cur_pid);
    void *pgd = get_task_pgd(pid);

    set_task_pgd(cur_pid, pgd);

    chunk_struct *whole_heap_chunk = (chunk_struct*)(heap + 16);
    whole_heap_chunk->size = get_end_heap(pid) - get_start_heap(pid);
    whole_heap_chunk->used = false;
    whole_heap_chunk->next = NULL;
    
    sprintf (buffer, "Heap Size Sum = %d", whole_heap_chunk->size);
    linfo (buffer);

    chunk_struct *free_chunk_head = (chunk_struct*)heap;
    free_chunk_head->size = -1;
    free_chunk_head->used = false;
    free_chunk_head->next = whole_heap_chunk;

    set_task_pgd(cur_pid, origin_pgd);

    return true;
}

void *tmalloc(int size, int pid) {
    char buffer[256];

    void *heap = get_start_heap(pid);

    int cur_pid = get_current_pid();
    void *origin_pgd = get_task_pgd(cur_pid);
    void *pgd = get_task_pgd(pid);

    set_task_pgd(cur_pid, pgd);

    chunk_struct *free_chunk = ((chunk_struct*)heap)->next;
    chunk_struct *last = (chunk_struct*)heap;

    sprintf (buffer, "search start: %P, list start: %P", free_chunk, last);
    linfo (buffer);

    //修正大小
    int fix_size = ((size / HEAPUNIT_SZ) + (size % HEAPUNIT_SZ != 0)) * HEAPUNIT_SZ;

    sprintf (buffer, "size = %d ; fixed size=%d", size, fix_size);
    linfo (buffer);

    //寻找符合条件的chunk
    while (free_chunk->size < (fix_size + 16) && (free_chunk->next != NULL)) {
        last = free_chunk;
        free_chunk = free_chunk->next;
    }

    sprintf (buffer, "free chunk: %P, size=%d, last: %P", free_chunk, free_chunk->size, last);
    linfo (buffer);

    if (free_chunk->size < (fix_size + 16)) {
        linfo ("Heap is running out!");
        //TODO: extend the heap
        return NULL;
    }

    //进行分割
    if (free_chunk->size > HEAPDIV_MIN_SZ && ((free_chunk->size - 32) > (fix_size + 16))) {
        chunk_struct *new_chunk = ((void*)free_chunk) + 16 + fix_size;
        new_chunk->next = free_chunk->next;
        new_chunk->size = free_chunk->size - 16 - fix_size;
        new_chunk->used = false;
        sprintf (buffer, "New chunk = %P ; size = %d ; next = %P", new_chunk, new_chunk->size, new_chunk->next);
        linfo (buffer);
        last->next = new_chunk;

        free_chunk->size = fix_size + 16;
    }
    else {
        last->next = NULL;
    }

    //更改信息
    free_chunk->used = true;
    free_chunk->next = NULL;

    set_task_pgd(cur_pid, origin_pgd);

    return ((void*)free_chunk) + 16;
}

void tfree(void *addr, int pid) {
    char buffer[256];
    
    void *heap = get_start_heap(pid);

    int cur_pid = get_current_pid();
    void *origin_pgd = get_task_pgd(cur_pid);
    void *pgd = get_task_pgd(pid);

    set_task_pgd(cur_pid, pgd);

    //当前chunk
    chunk_struct *chunk = (chunk_struct*)(addr - 16);
    if (! chunk->used) {
        lwarn ("try to free a unused chunk!");
        return;
    }

    sprintf (buffer, "free chunk %P", chunk);
    linfo (buffer);

    //空闲chunk链表头
    chunk_struct *free_chunk = ((chunk_struct*)heap)->next;
    chunk_struct *parent = (chunk_struct*)heap;

    //设为未使用
    chunk->used = false;
    chunk->next = NULL;

    while (free_chunk != NULL) {
        if (chunk > parent && chunk < free_chunk) {
            break;
        }
        parent = free_chunk;
        free_chunk = free_chunk->next;
    }

    sprintf (buffer, "last %P next %P", parent, free_chunk);
    linfo (buffer);

    parent->next = chunk; //插入
    chunk->next = free_chunk;

    if (parent != heap) {
        if ((((void*)parent) + parent->size) == chunk) {
            parent->size += chunk->size;
            parent->next = chunk->next;
            chunk = parent;
        }
    }

    if (free_chunk != NULL) {
        if ((((void*)chunk) + chunk->size) == free_chunk) {
            chunk->next = free_chunk->next;
            chunk->size += free_chunk->size;
        }
    }
    
    set_task_pgd(cur_pid, origin_pgd);
}