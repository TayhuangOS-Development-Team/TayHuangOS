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

//一个段的信息
struct __seg_info {
    size_t size : 63;
    bool used : 1;
    struct __seg_info *next;
    struct __seg_info *last;
    //地址
    char addr[0]; 
} __attribute__((packed));

typedef struct __seg_info seg_info_struct;

//分配单元 (64字节对齐(加上seg_info后))
#define MIN_SEG_SIZE (64)

//初始化kheap
PUBLIC void init_kheap(void *prepare_base, size_t prepare_size) {
    start_addr = prepare_base;
    seg_info_struct *start_seg = (seg_info_struct *)prepare_base; 

    start_seg->size = prepare_size;
    start_seg->used = false;
    start_seg->next = NULL;
    start_seg->last = NULL;
}

//分配 (1 << 6)个页 即 2^6 * 4096 = 256 KB
#define ALLOC_PAGE_ORDER (6)

PRIVATE seg_info_struct *do_combine(seg_info_struct *seg) {
    if ((seg->next != NULL) && (! seg->next->used)) {
        void *last_addr = seg;
        void *next_addr = seg->next;
        if (last_addr + seg->size == next_addr) {
            seg->size += seg->next->size;
            seg->next = seg->next->next;
            if (seg->next != NULL) {
                seg->next->last = seg;
            }
        }
    }

    if ((seg->last != NULL) && (! seg->last->used)) {
        void *last_addr = seg->last;
        void *next_addr = seg;
        if (last_addr + seg->last->size == next_addr) {
            seg->last->size += seg->size;
            seg->last->next = seg->next;
            if (seg->next != NULL) {
                seg->next->last = seg->last;
            }
            return seg->last;
        }
    }

    return seg;
}

PUBLIC void *kmalloc(size_t size) {
    size += sizeof(seg_info_struct);

    //修正大小
    size_t fixed_size = (size + (MIN_SEG_SIZE - 1)) & ~(MIN_SEG_SIZE - 1);

    seg_info_struct *cur_seg = (seg_info_struct *)start_addr;

    while ((cur_seg->size < fixed_size || cur_seg->used) && cur_seg->next != NULL) {
        cur_seg = cur_seg->next;
    }

    if ((cur_seg->size < fixed_size) || (cur_seg->used)) {
        //ask pmm for more memories
        linfo ("KHeap", "Ask pmm for more memories!");

        int order_give = 0;
        //因为在给pmm归还内存时pmm会用到这些内存 所以要先分配
        void *addr = __alloc_free_pages(ALLOC_PAGE_ORDER, &order_give); 
        
        seg_info_struct *seg = (seg_info_struct *)addr;
        seg->size = (MEMUNIT_SZ << ALLOC_PAGE_ORDER);
        seg->used = false;
        seg->next = NULL;

        seg->last = cur_seg;
        cur_seg->next = seg;

        //新内存加入堆中
        cur_seg = do_combine(seg); 

        //返还多余内存
        return_pages(((void *)seg) + (MEMUNIT_SZ << ALLOC_PAGE_ORDER), (1 << order_give) - (1 << ALLOC_PAGE_ORDER)); 

        return kmalloc(size);
    }

    if ((cur_seg->size - fixed_size) >= MIN_SEG_SIZE) {
        //进行分割
        seg_info_struct *new_seg = (seg_info_struct *)(((void *)cur_seg) + fixed_size);
        //新段大小
        new_seg->size = cur_seg->size - fixed_size; 
        //插入
        new_seg->next = cur_seg->next;

        if (cur_seg->next != NULL) {
            cur_seg->next->last = new_seg;
        }
        
        //未使用
        new_seg->used = false;
        new_seg->last = cur_seg;

        //用来段的大小
        cur_seg->size = fixed_size;
        cur_seg->next = new_seg;
        cur_seg->used = true;
    }
    else {
        cur_seg->used = true;
    }

    //返回地址
    return (void *)cur_seg->addr;
}

PUBLIC void kfree(void *ptr) {
    seg_info_struct *seg = (seg_info_struct *)(ptr - sizeof(seg_info_struct));
    //设置可用
    seg->used = false; 

    //合并
    do_combine(seg);
}