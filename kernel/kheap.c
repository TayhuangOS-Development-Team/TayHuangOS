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
 * kheap.c
 *
 * 内核堆
 *
 */



#include "kheap.h"

typedef struct {
    void *start;
    void *size;
} kh_seg; //内存项信息

#define KH_SEG_NUM (8192) //内存项数
PRIVATE kh_seg *KHEAP_SEGMENTS = NULL; //内存项表
PRIVATE void *KHEAP_TOP = NULL; //堆顶
PRIVATE void *KHEAP_BOTTOM = NULL; //堆底

PUBLIC void init_kheap(_IN void *kheap_limit) { //初始化堆
    KHEAP_SEGMENTS = (kh_seg*)(kheap_limit - sizeof(kh_seg) * KH_SEG_NUM);
    KHEAP_TOP = (void*)(kheap_limit - sizeof(kh_seg) * KH_SEG_NUM);
    KHEAP_BOTTOM = (void*)KHEAP_BASE;
    memset(KHEAP_SEGMENTS, 0, sizeof(kh_seg) * KH_SEG_NUM);
}

PRIVATE void *__get_segment_size(_IN void *base, _IN int size) { //获得项大小
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if (max(KHEAP_SEGMENTS[i].start, base) < min(KHEAP_SEGMENTS[i].size, (base + size))) {
            return KHEAP_SEGMENTS[i].size;
        }
    }
    return NULL;
}

PRIVATE void *__lookup_free_mem(_IN int size) { //寻找空闲内存
    for (void *i = KHEAP_BOTTOM ; i < KHEAP_TOP ;) {
        void *sz = __get_segment_size(i, size); //获取这个项的大小
        if (sz == NULL) //没大小:空闲内存
            return i;
        i = sz + 1; //加上size继续寻找
    }
    return NULL;
}

PRIVATE int __lookup_free_kh_seg(void) { //查找未被使用的内存项
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if ((KHEAP_SEGMENTS[i].start == NULL) && (KHEAP_SEGMENTS[i].size == NULL)) {
            return i;
        }
    }
    return -1;
}

PRIVATE bool __insert_kh_seg(_IN void *start, _IN void *size) { //插入内存项
    int idx = __lookup_free_kh_seg(); //寻找空闲内存项
    if (idx == -1) {
        return false;
    }
    KHEAP_SEGMENTS[idx].start = start;
    KHEAP_SEGMENTS[idx].size = size; //设置
    return true;
}

PRIVATE void __delete_kh_seg(_IN void *start) { //删除内存项
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if ((KHEAP_SEGMENTS[i].start == start)) {
            KHEAP_SEGMENTS[i].start = NULL;
            KHEAP_SEGMENTS[i].size = NULL; //设0
            break;
        }
    }
}

PUBLIC void *malloc(_IN int size) { //分配内存
    void *mem = __lookup_free_mem(size);
    if (mem == NULL)
        return NULL;
    if (! __insert_kh_seg(mem, mem + size - 1))
        return NULL;
    return mem;
}

PUBLIC void free(_IN void *ptr) { //释放内存
    __delete_kh_seg(ptr);
}