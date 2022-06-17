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
 * fifo_pool.c
 * 
 * FIFO POOL
 * 
 */



#include "fifo_pool.h"
#include "paging.h"
#include "pmm.h"

#include <kheap.h>
#include <debug/logging.h>
#include <display/printk.h>

PUBLIC void mapping_fifo(void) {
    set_mapping(FIFO_ADDRESS, FIFO_ADDRESS, FIFO_SIZE / 4096, true, true);
}

PUBLIC void mark_fifo_used(void) {
    for (int i = 0 ; i < FIFO_SIZE ; i += 4096) { //设置FIFO已被使用
        mark_used(FIFO_ADDRESS + i);
    }
}

typedef struct __fifo_area {
    void *address;
    int size : 31;
    bool used : 1;
    struct __fifo_area *next;
} fifo_area;

PRIVATE fifo_area *areas = NULL;

PUBLIC void init_fifo(void) {
    mark_fifo_used();
    mapping_fifo();
    areas = kmalloc(sizeof(fifo_area));
    areas->address = FIFO_ADDRESS;
    areas->size = FIFO_SIZE;
    areas->used = false;
    areas->next = NULL;
}

#define FIFO_UNIT (1024)

PUBLIC void *alloc_fifo(int size) {
    char buffer[160];

    int fixed_size = ((size / FIFO_UNIT) + ((size % FIFO_UNIT) != 0)) * FIFO_UNIT;
    
    sprintk (buffer, "require size = %d ; fixed size = %d", size, fixed_size);
    linfo ("FIFO", buffer);

    fifo_area *current = areas;
    while (current != NULL && (current->size < fixed_size || current->used)) {
        current = current->next;
    }

    if (current == NULL) {
        lerror ("FIFO", "There's no more fifo!");
        return NULL;
    }
    
    if (current->size != fixed_size) {
        fifo_area *new_area = kmalloc(sizeof(fifo_area));
        new_area->address = current->address + fixed_size;
        new_area->size = current->size - fixed_size;
        new_area->next = current->next;
        new_area->used = false;
        current->next = new_area;
        current->size = fixed_size;
        
        sprintk (buffer, "Split the area into (start = %P len = %d) and (start = %P len = %d)",
                        current->address, current->size, new_area->address, new_area->size);
        linfo ("FIFO", buffer);
    }
        
    current->used = true;

    sprintk (buffer, "Alloc chunk(start = %P, len = %d)", current->address, current->size);
    linfo ("FIFO", buffer);
    return current->address;
}

PUBLIC void free_fifo(void *addr) {
    char buffer[160];

    fifo_area *last = NULL;
    fifo_area *current = areas;
    while (current != NULL && current->address != addr) {
        last = current;
        current = current->next;
    }

    if (current == NULL) {
        lwarn ("FIFO", "Try to free an invalid fifo!");
        return;
    }

    current->used = false;

    if (last != NULL) {
        if (! last->used) {
            last->size += current->size;
            last->next = current->next;

            sprintk (buffer, "Combine %P and %P", last->address, current->address);
            linfo ("FIFO", buffer);

            kfree(current);
            current = last;
        }
    }

    if (current->next != NULL) {
        if (! current->next->used) {
            current->size += current->next->size;
            fifo_area *new_next = current->next->next;

            sprintk (buffer, "Combine %P and %P", current->address, current->next->address);
            linfo ("FIFO", buffer);

            kfree(current->next);

            current->next = new_next;
        }
    }
}