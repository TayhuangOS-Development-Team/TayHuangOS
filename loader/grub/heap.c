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

/**
 * @brief 堆
 * 
 */
PRIVATE byte HEAP[HEAP_SIZE];

/**
 * @brief 块信息
 * 
 */
struct __block_t {
    /**
     * @brief 大小
     * 
     */
    size_t size : 31;
    /**
     * @brief 是否使用过
     * 
     */
    bool used : 1;
    /**
     * @brief 下个块
     * 
     */
    struct __block_t *next;
    /**
     * @brief 上个块
     * 
     */
    struct __block_t *last;
    /**
     * @brief 地址
     * 
     */
    char addr[0]; 
} __attribute__((packed));

typedef struct __block_t block_t;

//分配单元 (16字节对齐(加上block_t后))
#define MIN_SEG_SIZE (16)

//初始化 mm
PUBLIC void mm_init(void) {
    block_t *start_block = (block_t *)HEAP; 

    start_block->size = HEAP_SIZE;
    start_block->used = false;
    start_block->next = NULL;
    start_block->last = NULL;
}

PRIVATE block_t *do_combine(block_t *block) {
    if ((block->next != NULL) && (! block->next->used)) {
        void *last_addr = block;
        void *next_addr = block->next;
        if (last_addr + block->size == next_addr) {
            block->size += block->next->size;
            block->next = block->next->next;
            if (block->next != NULL) {
                block->next->last = block;
            }
        }
    }

    if ((block->last != NULL) && (! block->last->used)) {
        void *last_addr = block->last;
        void *next_addr = block;
        if (last_addr + block->last->size == next_addr) {
            block->last->size += block->size;
            block->last->next = block->next;
            if (block->next != NULL) {
                block->next->last = block->last;
            }
            return block->last;
        }
    }

    return block;
}

PUBLIC NULLABLE void *malloc(size_t size) {
    size += sizeof(block_t);

    //修正大小
    size_t fixed_size = (size + (MIN_SEG_SIZE - 1)) & ~(MIN_SEG_SIZE - 1);

    block_t *cur_block = (block_t *)HEAP;

    while ((cur_block->size < fixed_size || cur_block->used) && cur_block->next != NULL) {
        cur_block = cur_block->next;
    }

    if ((cur_block->size < fixed_size) || (cur_block->used)) {
        LERROR ("GRUB2 Loader", "No more heap!");
        return NULL;
    }

    if ((cur_block->size - fixed_size) >= MIN_SEG_SIZE) {
        //进行分割
        block_t *new_block = (block_t *)(((void *)cur_block) + fixed_size);
        //新块大小
        new_block->size = cur_block->size - fixed_size; 
        //插入
        new_block->next = cur_block->next;

        if (cur_block->next != NULL) {
            cur_block->next->last = new_block;
        }
        
        //未使用
        new_block->used = false;
        new_block->last = cur_block;

        //当前块的大小
        cur_block->size = fixed_size;
        cur_block->next = new_block;
        cur_block->used = true;
    }
    else {
        cur_block->used = true;
    }

    //返回地址
    return (void *)cur_block->addr;
}

PUBLIC void free(NONNULL void *ptr) {
    block_t *block = (block_t *)(ptr - sizeof(block_t));
    //设置可用
    block->used = false; 

    //合并
    do_combine(block);
}