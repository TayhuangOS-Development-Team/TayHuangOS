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
 * kheap.c
 *
 * 内核堆
 *
 */



#include <memory/kheap.h>
#include <memory/pmm.h>

#include <string.h>

#include <logging.h>

PRIVATE void *start_addr = NULL;

struct __seg_info {
    qword size : 63;
    bool used : 1;
    struct __seg_info *next;
    struct __seg_info *last;
} __attribute__((packed));

typedef struct __seg_info seg_info_struct;

#define MIN_SEG_SIZE (64)

PUBLIC void init_kheap(void *prepare_base, int prepare_size) {
    start_addr = prepare_base;
    seg_info_struct *start_seg = (seg_info_struct*)prepare_base;

    start_seg->size = prepare_size;
    start_seg->used = false;
    start_seg->next = NULL;
    start_seg->last = NULL;
}

#define ALLOC_PAGE_ORDER (6)

PRIVATE seg_info_struct *extend_heap(int *order_give) {
    void *addr = __alloc_free_pages(ALLOC_PAGE_ORDER, order_give);
    seg_info_struct *seg = (seg_info_struct*)addr;
    seg->size = (MEMUNIT_SZ << ALLOC_PAGE_ORDER);
    seg->used = false;
    seg->next = NULL;
    return seg;
}

PRIVATE seg_info_struct *do_combine(seg_info_struct *seg) {
    if ((seg->next != NULL) && (! seg->next->used)) {
        seg->size += seg->next->size;
        seg->next = seg->next->next;
        if (seg->next != NULL) {
            seg->next->last = seg;
        }
    }

    if ((seg->last != NULL) && (! seg->last->used)) {
        seg->last->size += seg->size;
        seg->last->next = seg->next;
        if (seg->next != NULL) {
            seg->next->last = seg->last;
        }
        return seg->last;
    }

    return seg;
}

PUBLIC void *kmalloc(int size) {
    size += sizeof(seg_info_struct);

    int fixed_size = (size + (MIN_SEG_SIZE - 1)) & ~(MIN_SEG_SIZE - 1);

    seg_info_struct *cur_seg = (seg_info_struct*)start_addr;

    while ((cur_seg->size < fixed_size || cur_seg->used) && cur_seg->next != NULL) {
        cur_seg = cur_seg->next;
    }

    if (cur_seg->size < fixed_size || cur_seg->used) {
        //ask pmm for more memories
        linfo ("KHeap", "Ask pmm for more memories!");
        int order_give = 0;
        seg_info_struct *seg = extend_heap(&order_give);
        seg->last = cur_seg;
        cur_seg->next = seg;

        cur_seg = do_combine(seg);

        return_pages(((void*)seg) + (MEMUNIT_SZ << ALLOC_PAGE_ORDER), (1 << order_give) - (1 << ALLOC_PAGE_ORDER));

        return kmalloc(size);
    }

    if ((cur_seg->size - fixed_size) >= MIN_SEG_SIZE) {
        //进行分割
        seg_info_struct *new_seg = (seg_info_struct*)(((void*)cur_seg) + fixed_size);
        new_seg->size = cur_seg->size - fixed_size;
        new_seg->next = cur_seg->next;
        if (cur_seg->next != NULL) {
            cur_seg->next->last = new_seg;
        }
        new_seg->used = false;
        new_seg->last = cur_seg;

        cur_seg->size = fixed_size;
        cur_seg->next = new_seg;
        cur_seg->used = true;
    }
    else {
        cur_seg->used = true;
    }

    return (void*)(cur_seg) + sizeof(seg_info_struct);
}

PUBLIC void kfree(void *ptr) {
    seg_info_struct *seg = (seg_info_struct*)(ptr - sizeof(seg_info_struct));
    seg->used = false;

    do_combine(seg);
}