/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * memory.h
 *
 * 内存管理函数
 *
 */



#include "memory.h"
#include "printf.h"


PUBLIC void memcpy(void *dst, void *src, int sz) { //复制内存
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = *(char*)(src + i);
    }
}

PUBLIC void memset(void *dst, byte val, int sz) { //设置内存
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = val;
    }
}

struct {
    void *top;
    void *bottom;
    void *limit;
    void *mem_map;
    void *mem_table;
} HEAP_INFO; //堆信息

struct mem_entry {
    dword start;
    dword size;
}__attribute__((packed)); //内存项信息

#define MEM_ENTRY_NUM (1024) //内存项数
#define HEAP_MAX_SIZE (128 * 1024) //堆最大大小
#define __HEAP_START (0x9FFFF)
#define __HEAP_LIMIT (__HEAP_START - HEAP_MAX_SIZE)
PRIVATE int HEAP_START = __HEAP_START;

PUBLIC void init_heap(int memsz) { //初始化堆
    if (__HEAP_START >= (memsz - 0x400)) { //内存没那么大
        HEAP_START = (memsz - 0x400);
    }
    HEAP_INFO.mem_map = HEAP_START - HEAP_MAX_SIZE / 8; //使用情况位图
    HEAP_INFO.mem_table = HEAP_INFO.mem_map - MEM_ENTRY_NUM * sizeof(struct mem_entry); //内存项表
    HEAP_INFO.bottom = HEAP_INFO.top = HEAP_INFO.mem_table; //可用区域
    HEAP_INFO.limit = __HEAP_LIMIT; //堆底
    memset(HEAP_INFO.mem_map, 0, HEAP_MAX_SIZE / 8); //位图清0
    memset(HEAP_INFO.mem_table, 0, (MEM_ENTRY_NUM * sizeof(struct mem_entry))); //内存项表清0
}

PUBLIC void terminate_heap(int memsz) { //析构堆
    if (__HEAP_START >= (memsz - 0x400)) {
        HEAP_START = (memsz - 0x400);
    }
    HEAP_INFO.bottom = HEAP_INFO.top = HEAP_START + HEAP_MAX_SIZE / 8 + MEM_ENTRY_NUM * sizeof(struct mem_entry);
    HEAP_INFO.limit = __HEAP_LIMIT;
    memset(HEAP_INFO.mem_map, 0, HEAP_MAX_SIZE / 8); //位图清0
    memset(HEAP_INFO.mem_table, 0, (MEM_ENTRY_NUM * sizeof(struct mem_entry))); //内存项表清0
}

PUBLIC void reset_heap(int memsz) { //重置堆
    terminate_heap(memsz); //析构
    init_heap(memsz); //初始化
}

PRIVATE inline bool __query_using(void *ptr) { //是否有被使用
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top) {
        return true;
    }
    int no_of_ptr = HEAP_INFO.top - ptr;
    return (*(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) >> (no_of_ptr & 0x7)) & 0x1;
}

PUBLIC bool query_using(void *ptr, int size) { //这一段是否有被使用
    for (int i = 0 ; i < size ; i ++) {
        if (__query_using(ptr + i)) {
            return true;
        }
    }
    return false;
}

PRIVATE inline void ___set_using(void *ptr) { //设置被使用
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) = origin | (1 << (no_of_ptr & 0x7));
}

PRIVATE void __set_using(void *ptr, int size) { //设置这一段被使用
    for (int i = 0 ; i < size ; i ++)
        ___set_using(ptr + i);
}

PRIVATE inline void ___set_free(void *ptr) { //设置未被使用
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) = origin & (~(1 << (no_of_ptr & 0x7)));
}

PRIVATE void __set_free(void *ptr, int size) { //设置这一段未被使用
    for (int i = 0 ; i < size ; i ++)
        ___set_free(ptr + i);
}

PRIVATE void *__extend_heap(int size) { //拓展堆的大小
    if (HEAP_INFO.bottom - size < HEAP_INFO.limit) return NULL;
    HEAP_INFO.bottom -= size;
    return HEAP_INFO.bottom + 1;
}

PRIVATE int __lookup_entry(void *start) { //查找内存项
    for (int i = 0 ; i < MEM_ENTRY_NUM ; i ++) {
        struct mem_entry *entry = HEAP_INFO.mem_table + i * sizeof (struct mem_entry);
        if (((void*)entry->start) == start) return i;
    }
    return -1;
}

PRIVATE void __insert_entry(struct mem_entry *entry) { //插入内存项
    int idx = __lookup_entry(NULL); //查找未被使用的内存项
    if (idx == -1) return; //没有
    struct mem_entry *_entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    _entry->size = entry->size; //设置大小
    _entry->start = entry->start; //设置起始位置
}

PRIVATE int __get_entry_size(void *start) { //获取某项大小
    int idx = __lookup_entry(start); //获取内存项
    if (idx == -1) return 0;
    struct mem_entry *entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    return entry->size; //返回大小
}

PRIVATE void __delete_entry(void *start) { //删除
    int idx = __lookup_entry(start);
    if (idx == -1) return;
    struct mem_entry *entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    entry->size = 0; //清0
    entry->start = NULL;
}

PRIVATE void *__lookup_free_memory(int size) { //寻找可用内存
    for (void *i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) {
        if (! query_using(i, size)) {
            return i;
        }
    }
    return __extend_heap(size);
}

PUBLIC void *malloc(int size) { //分配内存
    void *ptr = __lookup_free_memory(size); //找可用内存
    if (ptr == NULL) return NULL; //没有
    struct mem_entry entry;
    entry.start = ptr;
    entry.size = size;
    __insert_entry(&entry); //插入项
    __set_using(ptr, size); //设置为被使用
    return ptr;
}

PUBLIC void *calloc(int num, int size) { //分配num个size的内存
    return malloc(num * size);
}

PUBLIC void free(void *ptr) { //释放内存
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top)
        return;
    int size = __get_entry_size(ptr); //获取内存项大小
    __set_free(ptr, size); //设置为被使用
    __delete_entry(ptr); //删除
}

PUBLIC void set_heap_top(void *top) { //设置堆顶
    HEAP_INFO.top = top;
}

PUBLIC void set_heap_limit(void *limit) { //设置堆底
    HEAP_INFO.limit = limit;
}

PUBLIC int get_used(void) { //有多少被使用
    int num = 0;
    for (void *i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) {
        num += __query_using(i);
    }
    return num;
}

PUBLIC int get_remain(void) { //有多少没被使用
    return HEAP_INFO.top - HEAP_INFO.limit - get_used();
}