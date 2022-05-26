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

PRIVATE byte *pmpage_bitmap;
PRIVATE qword pmpage_bitmap_size = 0;
PRIVATE qword pmpage_num = 0;

PUBLIC void init_pmm(qword pmemsz) { //初始化PMM
    pmpage_num = pmemsz / 4096 + ((pmemsz % 4096) != 0);
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
                    return (i * 8 + j) * 4096;
                }
            }
        }
    }
    return NULL;
}

PUBLIC void mark_used(void *page){ //标记为被使用过
    qword _page = (qword)page;
    pmpage_bitmap[(_page / 4096 / 8)] |= (1 << ((_page / 4096) % 8));
}

PUBLIC void mark_unused(void *page) { //标记为未被使用
    qword _page = (qword)page;
    pmpage_bitmap[(_page / 4096 / 8)] &= ~(1 << ((_page / 4096) % 8));
}

// 0 < max <= 64
PUBLIC void *find_freepages(int max, int *found) {
    qword *qwd_bmp = (qword*)pmpage_bitmap;
    for (int i = 0 ; i < pmpage_bitmap_size / 8 ; i ++) {
        if (qwd_bmp[i] != 0xFFFFFFFFFFFFFFFF) { //没有全被用
            int start = 0;
            qword state = qwd_bmp[i]; //64个位
            while ((state & 1) != 0) { //最低位不为0
                state >>= 1;
                start ++;
            }
            //现在低位是0
            if (i + 1 < (pmpage_bitmap_size / 8)) {
                pmpage_bitmap[i] |= (qwd_bmp[i + 1] << start); //补充下一个64位
            }
            else {
                pmpage_bitmap[i] |= (0xFFFFFFFFFFFFFFFF << start);
            }
            int num = 0;
            while ((start & 1) != 1) { //直到低位是1
                state >>= 1;
                num ++; //可用内存数++
            }
            if (num >= max) {
                num = max;
            }
            *found = num; //返回找到的可用内存数
            return (i * 8 + start) * 4096; //返回可用内存地址
        }
    }
    return NULL;
}