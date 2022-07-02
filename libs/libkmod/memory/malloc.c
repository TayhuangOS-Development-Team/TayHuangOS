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



#include <memory/malloc.h>

#include <tayhuang/services.h>

#include <debug/logging.h>
#include <ipc/ipc.h>
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

static void *heap;

bool init_heap(void) {
    char buffer[128];

    int pid = get_current_pid();
    
    sprintf (buffer, "Initialize buffer for %d", pid);
    linfo (buffer);

    heap = get_start_heap(pid);

    sprintf (buffer, "Heap start address = %P", heap);
    linfo (buffer);

    int size = get_end_heap(pid) - heap;

    sprintf (buffer, "Heap size = %d", size);
    linfo (buffer);

    chunk_struct *list_start = (chunk_struct*)heap;
    chunk_struct *whole_heap = (chunk_struct*)(heap + + sizeof(chunk_struct));

    list_start->size = -1;
    list_start->used = false;
    list_start->next = whole_heap;

    whole_heap->size = size - + sizeof(chunk_struct);
    whole_heap->used = false;
    whole_heap->next = NULL;

    linfo ("Initialize finished!");

    return true;
}

void *malloc(int size) {
    char buffer[160];

    int fixed_size = (size & (HEAPUNIT_SZ - 1)) & ~(HEAPUNIT_SZ - 1);

    sprintf (buffer, "require size = %d ; fixed size = %d", size, fixed_size);
    linfo (buffer);

    chunk_struct *last = (chunk_struct*)heap;
    chunk_struct *current = last->next;

    sprintf (buffer, "searching start with last = %P and current = %P", last, current);
    linfo (buffer);

    while ((current != NULL) && (current->size < (fixed_size + sizeof(chunk_struct)))) {
        last = current;
        current = current->next;
    }

    sprintf (buffer, "searching end with last = %P and current = %P", last, current);
    linfo (buffer);

    if (current == NULL) {
        //TODO: extended the heap!
        lwarn ("Need more memories");
        return NULL;
    }

    if ((current->size > HEAPDIV_MIN_SZ) && 
        ((current->size - 2 * sizeof(chunk_struct) - fixed_size) >= HEAPUNIT_SZ)) {
        chunk_struct *new_free_chunk = (chunk_struct*)(((void*)current) + sizeof(chunk_struct) + fixed_size);
        new_free_chunk->size = current->size - sizeof(chunk_struct) - fixed_size;
        new_free_chunk->next = current->next;
        new_free_chunk->used = false;
        last->next = new_free_chunk;
        current->size = sizeof(chunk_struct) + fixed_size;
        
        sprintf (buffer, "Split the chunk into (start = %P len = %d) and (start = %P len = %d)",
                        current, current->size, new_free_chunk, new_free_chunk->size);
        linfo (buffer);
    }
    else {
        last->next = current->next;
    }
        
    current->used = true;
    current->next = NULL;

    sprintf (buffer, "Alloc chunk(start = %P, len = %d)", current, current->size);
    linfo (buffer);

    return ((void*)current) + 16;
}

void free(void *addr) {
    char buffer[160];

    chunk_struct *last = (chunk_struct*)heap;
    chunk_struct *current = (chunk_struct*)(addr - sizeof(chunk_struct));

    while ((last != NULL) && (! ((current > last) && (current < last->next)))) {
        last = last->next;
    }

    sprintf (buffer, "Insert chunk(%P) between chunk(%P) and chunk(%P)", current, last, last->next);
    linfo (buffer);

    current->next = last->next;
    last->next = current;

    if ((((void*)last) + last->size) == current) {
        sprintf (buffer, "Combine chunk(%P) and chunk(%P)", last, current);
        linfo (buffer);

        last->size += current->size;
        last->next = current->next;
        current = last;
    }

    if ((((void*)last) + last->size) == last->next) {
        sprintf (buffer, "Combine chunk(%P) and chunk(%P)", last, last->next);
        linfo (buffer);

        last->size += last->next->size;
        last->next = last->next->next;
    }
}