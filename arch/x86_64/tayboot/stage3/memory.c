// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/memory.h
 *
 * 内存管理函数
 */



#include "memory.h"
#include "printf.h"


PUBLIC void memcpy(void* dst, void* src, int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = *(char*)(src + i);
    }
}

PUBLIC void memset(void* dst, byte val, int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = val;
    }
}

struct {
    void* top;
    void* bottom;
    void* limit;
    void* mem_map;
    void* mem_table;
} HEAP_INFO;

struct mem_entry {
    dword start;
    dword size;
}__attribute__((packed));

#define MEM_ENTRY_NUM (1024)
#define HEAP_MAX_SIZE (128 * 1024)
#define __HEAP_START (0x9FFFF)
#define __HEAP_LIMIT (__HEAP_START - HEAP_MAX_SIZE)
PRIVATE int HEAP_START = __HEAP_START;

PUBLIC void init_heap(int memsz) {
    if (__HEAP_START >= (memsz - 0x400)) {
        HEAP_START = (memsz - 0x400);
    }
    HEAP_INFO.mem_map = HEAP_START;
    HEAP_INFO.mem_table = HEAP_START + HEAP_MAX_SIZE / 8;
    HEAP_INFO.bottom = HEAP_INFO.top = HEAP_START + HEAP_MAX_SIZE / 8 + MEM_ENTRY_NUM * sizeof(struct mem_entry);
    HEAP_INFO.limit = __HEAP_LIMIT;
    memset(HEAP_INFO.mem_map, 0, HEAP_MAX_SIZE / 8);
    memset(HEAP_INFO.mem_table, 0, (MEM_ENTRY_NUM * sizeof(struct mem_entry)));
}

PUBLIC void terminate_heap(int memsz) {
    if (__HEAP_START >= (memsz - 0x400)) {
        HEAP_START = (memsz - 0x400);
    }
    HEAP_INFO.bottom = HEAP_INFO.top = HEAP_START + HEAP_MAX_SIZE / 8 + MEM_ENTRY_NUM * sizeof(struct mem_entry);
    HEAP_INFO.limit = __HEAP_LIMIT;
    memset(HEAP_INFO.mem_map, 0, HEAP_MAX_SIZE / 8);
    memset(HEAP_INFO.mem_table, 0, (MEM_ENTRY_NUM * sizeof(struct mem_entry)));
}

PUBLIC void reset_heap(int memsz) {
    terminate_heap(memsz);
    init_heap(memsz);
}

PRIVATE inline bool __query_using(void* ptr) {
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top) {
        return true;
    }
    int no_of_ptr = HEAP_INFO.top - ptr;
    return (*(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) >> (no_of_ptr & 0x7)) & 0x1;
}

PUBLIC bool query_using(void* ptr, int size) {
    for (int i = 0 ; i < size ; i ++) {
        if (__query_using(ptr + i)) {
            return true;
        }
    }
    return false;
}

PRIVATE inline void ___set_using(void* ptr) {
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) = origin | (1 << (no_of_ptr & 0x7));
}

PRIVATE void __set_using(void* ptr, int size) {
    for (int i = 0 ; i < size ; i ++)
        ___set_using(ptr + i);
}

PRIVATE inline void ___set_free(void* ptr) {
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    *(char*)(HEAP_INFO.mem_map + (no_of_ptr >> 3)) = origin & (~(1 << (no_of_ptr & 0x7)));
}

PRIVATE void __set_free(void* ptr, int size) {
    for (int i = 0 ; i < size ; i ++)
        ___set_free(ptr + i);
}

PRIVATE void* __extend_heap(int size) {
    if (HEAP_INFO.bottom - size < HEAP_INFO.limit) return NULL;
    HEAP_INFO.bottom -= size;
    return HEAP_INFO.bottom + 1;
}

PRIVATE int __lookup_entry(void* start) {
    for (int i = 0 ; i < MEM_ENTRY_NUM ; i ++) {
        struct mem_entry *entry = HEAP_INFO.mem_table + i * sizeof (struct mem_entry);
        if (((void*)entry->start) == start) return i;
    }
    return -1;
}

PRIVATE void __insert_entry(struct mem_entry* entry) {
    int idx = __lookup_entry(NULL);
    if (idx == -1) return;
    struct mem_entry *_entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    _entry->size = entry->size;
    _entry->start = entry->start;
}

PRIVATE int __get_entry_size(void* start) {
    int idx = __lookup_entry(start);
    if (idx == -1) return 0;
    struct mem_entry *entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    return entry->size;
}

PRIVATE void __delete_entry(void* start) {
    int idx = __lookup_entry(start);
    if (idx == -1) return;
    struct mem_entry *entry = HEAP_INFO.mem_table + idx * sizeof (struct mem_entry);
    entry->size = 0;
    entry->start = NULL;
}

PRIVATE void* __lookup_free_memory(int size) {
    for (void* i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) {
        if (! query_using(i, size)) {
            return i;
        }
    }
    return __extend_heap(size);
}

PUBLIC void* malloc(int size) {
    void* ptr = __lookup_free_memory(size);
    if (ptr == NULL) return ptr;
    struct mem_entry entry;
    entry.start = ptr;
    entry.size = size;
    __insert_entry(&entry);
    __set_using(ptr, size);
    return ptr;
}

PUBLIC void* calloc(int num, int size) {
    return malloc(num * size);
}

PUBLIC void free(void* ptr) {
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top)
        return;
    int size = __get_entry_size(ptr);
    __set_free(ptr, size);
    __delete_entry(ptr);
}

PUBLIC void set_heap_top(void* top) {
    HEAP_INFO.top = top;
}

PUBLIC void set_heap_limit(void* limit) {
    HEAP_INFO.limit = limit;
}

PUBLIC int get_used(void) {
    int num = 0;
    for (void* i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) {
        num += __query_using(i);
    }
    return num;
}

PUBLIC int get_remain(void) {
    return HEAP_INFO.top - HEAP_INFO.limit - get_used();
}