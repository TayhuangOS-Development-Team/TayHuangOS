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
byte HEAP[HEAP_SIZE];

/**
 * @brief 空闲表项
 * 
 */
typedef struct _freeblk {
    int block_size;
    struct _freeblk *next;
} freeblk;

/**
 * @brief 获取有效部分大小
 * 
 * @param block_size 块大小
 * @return 有效部分大小
 */
PRIVATE int get_valid_size(int block_size) {
    return block_size - sizeof(freeblk);
}

void mm_init(void) {
    //大块 HEAP
    freeblk *block = (freeblk *)HEAP;
    block->block_size = HEAP_SIZE;
    block->next = NULL;
}

//TODO
void *malloc(int size) {
    return NULL;
}

//TODO
void free(void *addr) {

}