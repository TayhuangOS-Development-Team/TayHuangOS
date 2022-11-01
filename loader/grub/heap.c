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
    b32 block_size : 31;
    /**
     * @brief 可用
     * 
     */
    bool busy : 1;
    /**
     * @brief 下个空闲块
     * 
     */
    struct _memblk *next_free;
    /**
     * @brief 上个空闲块
     * 
     */
    struct _memblk *last_free;
    struct _memblk *last_blk;
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
    head->next_free = (memblk *)(HEAP + sizeof(memblk));
    head->last_free = NULL;
    head->last_blk = NULL;
    head->busy = false;
    //大块 HEAP
    memblk *heap_block = (memblk *)(HEAP + sizeof(memblk));
    heap_block->block_size = HEAP_SIZE;
    heap_block->next_free = NULL;
    heap_block->last_free = head;
    heap_block->last_blk = head;
    heap_block->busy = false;
}

#define HEAP_TOP (HEAP + HEAP_SIZE)

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
        if (current->next_free == NULL) {
            return NULL;
        }
        //下一个块
        current = current->next_free;
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
 * @brief 获得下一个块
 * 
 * @param last 上一个块
 * @return 下一个块 
 */
INLINE memblk *get_next_block(memblk *last) {
    void *addr = ((void *)last) + last->block_size;
    if (addr >= (void *)HEAP_TOP) {
        return NULL;
    }
    return (memblk *)addr;
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
    
    rest_blk->next_free = origin->next_free;
    rest_blk->last_blk = rest_blk->last_free = origin;
    rest_blk->block_size = rest_size;
    rest_blk->busy = false;
    
    origin->next_free = rest_blk;

    // 更新下一个空闲块的上一个空闲块
    if (rest_blk->next_free != NULL) {
        rest_blk->next_free->last_free = rest_blk;
    }

    // 更新下一个块的上一个块
    if (get_next_block(rest_blk) != NULL) {
        get_next_block(rest_blk)->last_blk = rest_blk;
    }
    
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
    memblk *lastblk = freeblk->last_free;
    memblk *nextblk = freeblk->next_free;
    
    //出队
    lastblk->next_free = nextblk;
    nextblk->last_free = lastblk;

    freeblk->last_free = freeblk->next_free = NULL;
    freeblk->busy = true;
    
    void *addr = (void *)freeblk;
    addr += sizeof(memblk);
    return addr;
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
    if (front == (memblk *)HEAP) {
        return false;
    }
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
    // back有一个空闲块
    if (back->next_free != NULL) {
        front->next_free = back->next_free;
        back->next_free->last_free = front;
    }
    else {
        front->next_free = NULL;
    }

    // back有下一个块
    if (get_next_block(back) != NULL) {
        get_next_block(back)->last_blk = front;
    }

    front->block_size += back->block_size;
    return front;
}

PUBLIC void free(void *addr) {
    //不可释放NULL
    if (addr == NULL) {
        LERROR("GRUB2 Loader MM", "Try to free NULL");
        return;
    }
    memblk *blk = (memblk *)(addr - sizeof(memblk));
    //该块已是可用块
    if (! blk->busy) {
        LERROR("GRUB2 Loader MM", "Try to free avaliable space");
        return;
    }
    //寻找上一个可用块
    memblk *lastblk = blk->last_blk;
    while (lastblk != NULL) {
        if (! lastblk->busy) {
            break;
        }
        lastblk = lastblk->last_blk;
    }

    //意外情况
    if (lastblk == NULL) {
        LERROR("GRUB2 Loader MM", "Last block shouldn't be NULL");
        return;
    } 

    //获得下一个可用块
    memblk *nextblk = lastblk->next_free; 
    
    //入队
    lastblk->next_free = blk;
    blk->last_free = lastblk;

    blk->next_free = nextblk;

    //有下一个块
    if (nextblk != NULL) {
        nextblk->last_free = blk;
    }

    //设置为可用块
    blk->busy = false;
    if (nextblk != NULL) {
        //合并块
        if (should_merge(blk, nextblk)) {
            merge_block(blk, nextblk);
        }
    }
    //合并块
    if (should_merge(lastblk, blk)) {
        merge_block(lastblk, blk);
    }
}