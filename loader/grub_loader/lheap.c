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
 * lheap.c
 *
 * Loader堆
 *
 */



#include <lheap.h>
#include <string.h>

typedef struct {
    void *start;
    void *size;
} lh_seg; //内存项信息

#define LH_SEG_NUM (8192) //内存项数
PRIVATE lh_seg *LHEAP_SEGMENTS = NULL; //内存项表
PRIVATE void *LHEAP_TOP = NULL; //堆顶
PRIVATE void *LHEAP_BOTTOM = NULL; //堆底

PUBLIC void init_lheap(void *lheap_size) { //初始化堆
    LHEAP_SEGMENTS = (lh_seg*)(LHEAP_BASE + lheap_size - sizeof(lh_seg) * LH_SEG_NUM);
    LHEAP_TOP = (void*)(LHEAP_BASE + lheap_size - sizeof(lh_seg) * LH_SEG_NUM);
    LHEAP_BOTTOM = (void*)LHEAP_BASE;
    memset(LHEAP_SEGMENTS, 0, sizeof(lh_seg) * LH_SEG_NUM);
}

PRIVATE void *__get_segment_size(void *base, int size) { //获得项大小
    for (int i = 0 ; i < LH_SEG_NUM ; i ++) {
        if (max(LHEAP_SEGMENTS[i].start, base) < min(LHEAP_SEGMENTS[i].size, (base + size))) {
            return LHEAP_SEGMENTS[i].size;
        }
    }
    return NULL;
}

PRIVATE void *__lookup_free_mem(int size) { //寻找空闲内存
    for (void *i = LHEAP_BOTTOM ; i < LHEAP_TOP ;) {
        void *sz = __get_segment_size(i, size); //获取这个项的大小
        if (sz == NULL) //没大小:空闲内存
            return i;
        i = sz + 1; //加上size继续寻找
    }
    return NULL;
}

PRIVATE int __lookup_free_lh_seg(void) { //查找未被使用的内存项
    for (int i = 0 ; i < LH_SEG_NUM ; i ++) {
        if ((LHEAP_SEGMENTS[i].start == NULL) && (LHEAP_SEGMENTS[i].size == NULL)) {
            return i;
        }
    }
    return -1;
}

PRIVATE bool __insert_lh_seg(void *start, void *size) { //插入内存项
    int idx = __lookup_free_lh_seg(); //寻找空闲内存项
    if (idx == -1) {
        return false;
    }
    LHEAP_SEGMENTS[idx].start = start;
    LHEAP_SEGMENTS[idx].size = size; //设置
    return true;
}

PRIVATE void __delete_lh_seg(void *start) { //删除内存项
    for (int i = 0 ; i < LH_SEG_NUM ; i ++) {
        if ((LHEAP_SEGMENTS[i].start == start)) {
            LHEAP_SEGMENTS[i].start = NULL;
            LHEAP_SEGMENTS[i].size = NULL; //设0
            break;
        }
    }
}

PUBLIC void *lmalloc(int size) { //分配内存
    void *mem = __lookup_free_mem(size);
    if (mem == NULL)
        return NULL;
    if (! __insert_lh_seg(mem, mem + size - 1))
        return NULL;
    return mem;
}

PUBLIC void lfree(void *ptr) { //释放内存
    __delete_lh_seg(ptr);
}