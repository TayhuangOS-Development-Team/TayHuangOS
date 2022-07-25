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
 * malloc.c
 * 
 * 分配内存
 * 
 */



#include <memory/malloc.h>
#include <debug/logging.h>

//一次分配16的倍数个内存
#define HEAPUNIT_SZ (16)
//小于等于该值时不再对区块进行分割
#define HEAPDIV_MIN_SZ (128)

//chunk起始地址 + 16B 即为数据所在地址
typedef struct _chunk {
    qword size : 63;
    bool used : 1;
    struct _chunk *next;
    char addr[0]; //地址
} chunk_struct; //16B
//堆开头16个Byte为空闲chunk头

PRIVATE void *heap;

PUBLIC bool init_heap(int pid, void *heap_start, void *heap_end) {
    linfo ("Initialize buffer for %d", pid);

    heap = heap_start;

    linfo ("Heap start address = %P", heap);

    int size = heap_end - heap;

    linfo ("Heap size = %d", size);

    chunk_struct *list_start = (chunk_struct*)heap;
    chunk_struct *whole_heap = (chunk_struct*)(heap + + sizeof(chunk_struct));

    //空闲链表头
    list_start->size = -1;
    list_start->used = false;
    list_start->next = whole_heap;

    //整个堆
    whole_heap->size = size - + sizeof(chunk_struct);
    whole_heap->used = false;
    whole_heap->next = NULL;

    linfo ("Initialize finished!");

    return true;
}

PUBLIC void *malloc(int size) {
    if (size == 0) {
        return NULL;
    }

    int fixed_size = (size + sizeof(chunk_struct) + (HEAPUNIT_SZ - 1)) & ~(HEAPUNIT_SZ - 1);

    chunk_struct *last = (chunk_struct*)heap;
    chunk_struct *current = last->next;

    while ((current != NULL) && (current->size < fixed_size)) {
        last = current;
        current = current->next;
    }

    if (current == NULL) {
        //TODO: extended the heap!
        lwarn ("Need more memories");
        return NULL;
    }

    int remain_size = current->size - fixed_size;

    //分割
    if ((current->size > HEAPDIV_MIN_SZ) && (remain_size >= HEAPUNIT_SZ)) {
        chunk_struct *new_free_chunk = (chunk_struct*)(((void *)current) + sizeof(chunk_struct) + fixed_size);

        new_free_chunk->size = current->size - fixed_size;
        new_free_chunk->next = current->next;
        new_free_chunk->used = false;

        last->next = new_free_chunk;
        current->size = fixed_size;
    }
    else {
        last->next = current->next;
    }
        
    current->used = true;
    current->next = NULL;

    return current->addr;
}

PUBLIC void free(void *addr) {
    chunk_struct *last = (chunk_struct*)heap;
    chunk_struct *current = (chunk_struct*)(addr - sizeof(chunk_struct));

    while ((last != NULL) && (! ((current > last) && (current < last->next)))) {
        last = last->next;
    }

    current->next = last->next;
    last->next = current;

    //可以合并
    if ((((void *)last) + last->size) == current) {
        last->size += current->size;
        last->next = current->next;
        current = last;
    }

    //可以合并
    if ((((void *)last) + last->size) == last->next) {
        last->size += last->next->size;
        last->next = last->next->next;
    }
}