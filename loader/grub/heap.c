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
    bool busy : 1;
    /**
     * @brief 下个空闲块偏移
     * 
     */
    b32 next_free_offset : 21;
    /**
     * @brief 上个空闲块偏移
     * 
     */
    b32 last_free_offset : 21;
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
    head->last_free_offset = 0;
    head->busy = true;
    //大块 HEAP
    memblk *heap_block = (memblk *)(HEAP + sizeof(memblk));
    heap_block->block_size = HEAP_SIZE;
    heap_block->next_free_offset = 0;
    heap_block->last_free_offset = sizeof(memblk);
    heap_block->busy = false;
}

#define CALC_ADDR(base, offset) (((void*)base) + offset)
#define CALC_OFF(low, high) (((void*)high) - ((void*)low))
#define HEAP_TOP CALC_ADDR(HEAP, HEAP_SIZE)

/**
 * @brief 获得下个空闲内存块
 * 
 * @param block 当前块
 * @return 下个空闲内存块
 */
INLINE memblk *get_next_free_block(memblk *block) {
    return (memblk *)CALC_ADDR(block, block->next_free_offset);
}

/**
 * @brief 获得下个内存块
 * 
 * @param block 当前块
 * @return 下个内存块
 */
INLINE memblk *get_next_block(memblk *block) {
    return (memblk *)CALC_ADDR(block, block->block_size);
    
}

/**
 * @brief 获得上个内存块
 * 
 * @param block 当前块
 * @return 上个内存块
 */
INLINE memblk *get_last_free_block(memblk *block) {
    return (memblk *)CALC_ADDR(block, -block->last_free_offset);
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
        current = get_next_free_block(current);
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
    //分割后大小
    size_t new_size = sizeof(memblk) + expectation;
    //剩余块大小
    size_t rest_size = origin->block_size - new_size;
    origin->block_size = new_size;
    //剩余块
    memblk *rest_blk = get_next_block(origin);
    rest_blk->block_size = rest_size;
    rest_blk->busy = false;
    rest_blk->last_free_offset = new_size;
    //设置下个空闲块偏移
    if (origin->next_free_offset != 0) {
        rest_blk->next_free_offset = origin->next_free_offset - new_size;
    }
    else {
        rest_blk->next_free_offset = 0;
    }
    origin->next_free_offset = new_size;
    return rest_blk;
}

PUBLIC void *malloc(int size) {
    memblk *freeblk = lookup_free_block(size);
    if (freeblk == NULL) {
        LERROR("GRUB2 Loader MM", "Have no memory to alloc");
        return NULL;
    }
    //分割
    if (should_split(freeblk, size)) {
        split_block(freeblk, size);
    }
    //上下块
    memblk *lastblk = get_last_free_block(freeblk);
    memblk *nextblk = get_next_free_block(freeblk);
    //出队
    if (nextblk == NULL) {
        lastblk->next_free_offset = 0;
    }
    else {
        size_t offset = CALC_OFF(lastblk, nextblk);
        nextblk->last_free_offset = lastblk->next_free_offset = offset;
    }
    // 设置已用
    freeblk->busy = true;
    freeblk->next_free_offset = 0;
    freeblk->last_free_offset = 0;
    return CALC_ADDR(freeblk, sizeof(memblk));
}


/**
 * @brief 是否应该合并内存块
 * 
 * @param front 前面的内存块
 * @param back 后面的内存块
 * @return true 应该合并
 * @return false 不应该合并
 */
INLINE bool should_merge(memblk *front, memblk *back) {
    //有一个是不可用块
    if (front->busy || back->busy) {
        return false;
    }
    //两个块不相连
    if (get_next_block(front) != back) {
        return false;
    }
    return true;
}

/**
 * @brief 合并内存块
 * 
 * @param front 前面的内存块
 * @param back 后面的内存块
 * @return 合并后的内存块
 */
INLINE memblk *merge_block(memblk *front, memblk *back) {
    front->next_free_offset = back->next_free_offset;
    front->block_size += back->block_size;
    return front;
}

/**
 * @brief 寻找最后一个空闲块
 * 
 * @return 最后一个空闲块
 */
INLINE memblk *find_last_free(void) {
    memblk *freeblk = (memblk *)HEAP;
    //还存在下一个可用块
    while (freeblk->next_free_offset != 0) {
        get_next_free_block(freeblk);
    }
    return freeblk;
}

PUBLIC void free(void *addr) {
    //不可释放NULL
    if (addr == NULL) {
        LERROR("GRUB2 Loader MM", "Try to free NULL");
        return;
    }
    memblk *blk = CALC_ADDR(addr, -sizeof(memblk));
    //该块已是可用块
    if (! blk->busy) {
        LERROR("GRUB2 Loader MM", "Try to free avaliable space");
        return;
    }
    //寻找下一个可用块
    memblk *next_free = get_next_block(blk);
    //寻找下一个可用块
    bool found_free = false;
    while (((void*)next_free) < HEAP_TOP) {
        //是可用块
        if (! next_free->busy) {
            found_free = true;
            break;
        }
        //下一个块
        next_free = get_next_block(next_free);
    }
    //没有下一个可用块
    if (! found_free) {
        //获得最后一个可用块
        memblk *last_free = find_last_free();
        //入队
        last_free->next_free_offset = blk->last_free_offset = CALC_OFF(last_free, blk);
        //设置为可用块
        blk->busy = false;
        //应当合并
        if (should_merge(last_free, blk)) {
            merge_block(last_free, blk);
        }
        return;
    }
    //有下一个可用块
    //获得上一个可用块
    memblk *last_free = get_last_free_block(next_free);
    //入队
    last_free->next_free_offset = blk->last_free_offset = CALC_OFF(last_free, blk);
    next_free->last_free_offset = blk->next_free_offset = CALC_OFF(blk, next_free);
    //设置为可用块
    blk->busy = false;
    //合并块
    if (should_merge(blk, next_free)) {
        merge_block(blk, next_free);
    }
    //合并块
    if (should_merge(last_free, blk)) {
        merge_block(last_free, blk);
    }
}