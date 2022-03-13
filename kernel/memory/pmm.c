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
#include "../kheap.h"

PRIVATE byte *pmpage_bitmap;
PRIVATE qword pmpage_bitmap_size = 0;
PRIVATE qword pmpage_num = 0;

PUBLIC void init_pmm(qword pmemsz) {
    pmpage_num = pmemsz / 4096 + ((pmemsz % 4096) != 0);
    pmpage_bitmap_size = pmpage_num / 8 + ((pmpage_num % 8) != 0);
    pmpage_bitmap = malloc (pmpage_bitmap_size);
    memset(pmpage_bitmap, 0, pmpage_bitmap_size);
}

PUBLIC void *lookup_free_page(void){
    for (int i = 0 ; i < pmpage_bitmap_size ; i ++) {
        if (pmpage_bitmap[i] != 0xFF) {
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

PUBLIC void mark_used(void *page){
    qword _page = (qword)page;
    pmpage_bitmap[(_page / 4096 / 8)] |= (1 << ((_page / 4096) % 8));
}

PUBLIC void mark_unused(void *page) {
    qword _page = (qword)page;
    pmpage_bitmap[(_page / 4096 / 8)] &= ~(1 << ((_page / 4096) % 8));
}