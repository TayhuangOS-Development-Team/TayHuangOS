/**
 * @file heap.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 堆
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <heap.h>
#include <logging.h>
#include <string.h>

#include <string.h>

#include <logging.h>

PRIVATE byte HEAP[HEAP_SIZE];

//一个段的信息
struct __seg_t {
    size_t size : 31;
    bool used : 1;
    struct __seg_t *next;
    struct __seg_t *last;
    //地址
    char addr[0]; 
} __attribute__((packed));

typedef struct __seg_t seg_t;

//分配单元 (32字节对齐(加上seg_t后))
#define MIN_SEG_SIZE (16)

//初始化 mm
PUBLIC void mm_init(void) {
    seg_t *start_seg = (seg_t *)HEAP; 

    start_seg->size = HEAP_SIZE;
    start_seg->used = false;
    start_seg->next = NULL;
    start_seg->last = NULL;
}

PRIVATE seg_t *do_combine(seg_t *seg) {
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

PUBLIC NULLABLE void *malloc(size_t size) {
    size += sizeof(seg_t);

    //修正大小
    size_t fixed_size = (size + (MIN_SEG_SIZE - 1)) & ~(MIN_SEG_SIZE - 1);

    seg_t *cur_seg = (seg_t *)HEAP;

    while ((cur_seg->size < fixed_size || cur_seg->used) && cur_seg->next != NULL) {
        cur_seg = cur_seg->next;
    }

    if ((cur_seg->size < fixed_size) || (cur_seg->used)) {
        LERROR ("GRUB2 Loader", "No more heap!");
        return NULL;
    }

    if ((cur_seg->size - fixed_size) >= MIN_SEG_SIZE) {
        //进行分割
        seg_t *new_seg = (seg_t *)(((void *)cur_seg) + fixed_size);
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

PUBLIC void free(NONNULL void *ptr) {
    seg_t *seg = (seg_t *)(ptr - sizeof(seg_t));
    //设置可用
    seg->used = false; 

    //合并
    do_combine(seg);
}