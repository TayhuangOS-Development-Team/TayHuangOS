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
 * pmm.c
 *
 * 物理内存管理器
 *
 */



#include "pmm.h"
#include <kheap.h>
#include <tayhuang/paging.h>

PRIVATE byte *pmpage_bitmap;
PRIVATE qword pmpage_bitmap_size = 0;
PRIVATE qword pmpage_num = 0;

PUBLIC void init_pmm(qword pmemsz) { //初始化PMM
    pmpage_num = pmemsz / MEMUNIT_SZ + ((pmemsz % MEMUNIT_SZ) != 0);
    pmpage_bitmap_size = pmpage_num / 8 + ((pmpage_num % 8) != 0);
    pmpage_bitmap = malloc (pmpage_bitmap_size);
    memset(pmpage_bitmap, 0, pmpage_bitmap_size);
}

PUBLIC void *lookup_free_page(void){ //寻找空闲页
    for (int i = 0 ; i < pmpage_bitmap_size ; i ++) {
        if (pmpage_bitmap[i] != 0xFF) { //没有全被用
            for (int j = 0 ; j < 8 ; j ++) {
                if ((i * 8 + j) > pmpage_num) {
                    return NULL;
                }
                if ((pmpage_bitmap[i] & (1 << j)) == 0) {
                    return (i * 8 + j) * MEMUNIT_SZ;
                }
            }
        }
    }
    return NULL;
}

PUBLIC void mark_used(void *page){ //标记为被使用过
    qword _page = (qword)page;
    pmpage_bitmap[(_page / MEMUNIT_SZ / 8)] |= (1 << ((_page / MEMUNIT_SZ) % 8));
}

PUBLIC void mark_unused(void *page) { //标记为未被使用
    qword _page = (qword)page;
    pmpage_bitmap[(_page / MEMUNIT_SZ / 8)] &= ~(1 << ((_page / MEMUNIT_SZ) % 8));
}

// 0 < max <= 64
PUBLIC void *find_freepages(int max, int *found) {
    //can be improved
    int i, j;
    bool flag = false;
    for (i = 0 ; i < pmpage_bitmap_size ; i ++) {
        if (pmpage_bitmap[i] == 0xFF)
            continue;
        for (j = 0 ; j < 8 ; j ++) {    
            if ((i * 8 + j) > pmpage_num) {
                *found = 0;
                return NULL;
            }
            if ((pmpage_bitmap[i] & (1 << j)) == 0) {
                flag = true;
                break;
            }
        }
        if (flag)
            break;
    }
    if (! flag) {
        *found = 0;
        return NULL;
    }
    flag = false;
    int sum = 0;
    void *start = (i * 8 + j) * MEMUNIT_SZ; 
    for (; i < pmpage_bitmap_size ; i ++) {
        for (; j < pmpage_bitmap_size ; j ++) {
            if (pmpage_bitmap[i] & (1 << j) == 0) {
                sum ++;
                if (sum >= max) {
                    flag = true;
                    break;
                }
            }
            else {
                flag = true;
                break;
            }
        }
        if (flag)
            break;
        j = 0;
    }
    *found = sum;
    return start;
}