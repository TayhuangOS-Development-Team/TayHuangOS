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

/**
 * @brief 堆
 * 
 */
PUBLIC byte HEAP[HEAP_SIZE];

/**
 * @brief 内存表项
 * 64bit - 8B
 */
struct _memblk {
    /**
     * @brief 块大小
     * 
     */
    b32 block_size : 21;
    /**
     * @brief 可用
     * 
     */
    bool avaliable : 1;
    /**
     * @brief 下个空闲块偏移
     * 
     */
    b32 next_free_offset : 21;
    /**
     * @brief 上个空闲块偏移
     * 
     */
    b32 last_offset : 21;
} __attribute__((packed));

typedef struct _memblk memblk;

/**
 * @brief 获取有效部分大小
 * 
 * @param block_size 块大小
 * @return 有效部分大小
 */
PRIVATE size_t get_valid_size(size_t block_size) {
    return block_size - sizeof(memblk);
}

PUBLIC void mm_init(void) {
    //空闲块链表头
    memblk *head = (memblk *)HEAP;
    head->block_size = sizeof(memblk);
    head->next_free_offset = sizeof(memblk);
    head->last_offset = 0;
    head->avaliable = false;
    //大块 HEAP
    memblk *heap_block = (memblk *)(HEAP + sizeof(memblk));
    heap_block->block_size = HEAP_SIZE;
    heap_block->next_free_offset = 0;
    head->last_offset = sizeof(memblk);
    heap_block->avaliable = true;
}

#define CALC_ADDR(base, offset) (((void*)base) + offset)

/**
 * @brief 获得下个空闲内存块
 * 
 * @param block 当前块
 * @return 下个空闲内存块
 */
INLINE memblk *get_next_free(memblk *block) {
    return (memblk *)CALC_ADDR(block, block->next_free_offset);
}

/**
 * @brief 获得下个内存块
 * 
 * @param block 当前块
 * @return 下个内存块
 */
INLINE memblk *get_next(memblk *block) {
    return (memblk *)CALC_ADDR(block, block->block_size);
    
}

/**
 * @brief 获得上个内存块
 * 
 * @param block 当前块
 * @return 上个内存块
 */
INLINE memblk *get_last(memblk *block) {
    return (memblk *)CALC_ADDR(block, block->last_offset);
}

/**
 * @brief 寻找合适空闲块
 * 
 * @param expectation 预期可用大小
 * @return 合适的空闲块
 * @return NULL 没有合适空闲块
 */
INLINE memblk *lookup_free_block(size_t expectation) {
    memblk *current = (memblk *)HEAP;

    //直到找到大小合适的下一个块
    while (get_valid_size(current->block_size) < expectation) {
        //没有下一个块
        if (current->next_free_offset == 0) {
            return NULL;
        }
        //下一个块
        current = get_next_free(current);
    }

    return current;
}

/**
 * @brief 最低块大小
 * 
 */
#define LEAST_BLOCK_SIZE (16)

/**
 * @brief 是否应该分割内存块
 * 
 * @param block 内存块
 * @param expectation 预期可用大小
 * @return true 应该
 * @return false 不应该
 */
INLINE bool should_split(memblk *block, size_t expectation) {
    //块本身大小不得低于LEAST_BLOCK_SIZE
    if (block->block_size <= LEAST_BLOCK_SIZE) {
        return false;
    }
    //分割后剩余块大小不得低于LEAST_BLOCK_SIZE
    if (get_valid_size(block->block_size) - expectation <= LEAST_BLOCK_SIZE) {
        return false;
    }
    //应该分割
    return true;
}

/**
 * @brief 分割内存块
 * 
 * @param origin 原内存块
 * @param expectation 预期可用大小
 * @return 分割后的剩余内存块
 */
INLINE memblk *split_block(memblk *origin, size_t expectation) {

}

//TODO
PUBLIC void *malloc(int size) {
    return NULL;
}

//TODO
PUBLIC void free(void *addr) {

}