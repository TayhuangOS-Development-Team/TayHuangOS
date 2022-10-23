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
    void *start;
    struct _freeblk *next;
} freeblk;

//TODO
void *malloc(int size) {
    return NULL;
}

//TODO
void free(void *addr) {

}