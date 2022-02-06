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
 * arch/x86_64/tayboot/stage2/heap.h
 *
 * Heap manage functions are declared here
 *
 */



#include "heap.h"
#include "buffer.h"

//堆信息
PRIVATE struct {
    void *top;
    void *bottom;
    void *limit;
    addr_t mem_map;
    addr_t mem_table;
} HEAP_INFO;

//内存项
struct mem_entry {
    int start : 14;
    int size : 10;
}__attribute__((packed));

#define MEM_ENTRY_NUM (1024) //内存项数
#define HEAP_MAX_SIZE (16 * 1024) //堆最大大小

PUBLIC void init_heap(void) {
    HEAP_INFO.mem_map = alloc_buffer(HEAP_MAX_SIZE / 8, false); //分配mem_map与mem_table
    HEAP_INFO.mem_table = alloc_buffer(HEAP_MAX_SIZE * sizeof(struct mem_entry), false);
    HEAP_INFO.bottom = HEAP_INFO.top = 0xFFFF; //设置堆底堆顶
    HEAP_INFO.limit = 0xC000; //设置极限
    for (int i = 0 ; i < (HEAP_MAX_SIZE / 8) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_map + i , 0); //清空mem_map
    }
    for (int i = 0 ; i < (HEAP_MAX_SIZE * sizeof(struct mem_entry)) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_table + i , 0); //清空mem_table
    }
}

PUBLIC void terminate_heap(void) {
    HEAP_INFO.bottom = HEAP_INFO.top = 0xFFFF; //设置堆底堆顶
    HEAP_INFO.limit = 0xC000; //设置极限
    for (int i = 0 ; i < (HEAP_MAX_SIZE / 8) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_map + i , 0); //清空mem_map
    }
    for (int i = 0 ; i < (HEAP_MAX_SIZE * sizeof(struct mem_entry)) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_table + i , 0); //清空mem_table
    }
    free_buffer(HEAP_INFO.mem_map); //释放mem_map
    free_buffer(HEAP_INFO.mem_table); //释放mem_table
}

PUBLIC void reset_heap(void) {
    HEAP_INFO.bottom = HEAP_INFO.top = 0xFFFF; //设置堆底堆顶
    HEAP_INFO.limit = 0xC000; //设置极限
    for (int i = 0 ; i < (HEAP_MAX_SIZE / 8) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_map + i , 0); //清空mem_map
    }
    for (int i = 0 ; i < (HEAP_MAX_SIZE * sizeof(struct mem_entry)) ; i ++) {
        set_buffer_byte(HEAP_INFO.mem_table + i , 0); //清空mem_table
    }
}

PRIVATE inline bool __query_using(void *ptr) { //查询某个字节是否又被使用
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top) { //不在范围内
        return true;
    }
    int no_of_ptr = HEAP_INFO.top - ptr;
    return (get_buffer_byte(HEAP_INFO.mem_map + (no_of_ptr >> 3)) >> (no_of_ptr & 0x7)) & 0x1;
}

PUBLIC bool query_using(void *ptr, int size) {
    for (int i = 0 ; i < size ; i ++) { //一个个查询
        if (__query_using(ptr + i)) {
            return true;
        }
    }
    return false;
}

PRIVATE inline void ___set_using(void *ptr) { //设置某个字节被使用
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = get_buffer_byte(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    set_buffer_byte(HEAP_INFO.mem_map + (no_of_ptr >> 3), origin | (1 << (no_of_ptr & 0x7)));
}

PRIVATE void __set_using(void *ptr, int size) { //设置某段内存被使用
    for (int i = 0 ; i < size ; i ++) //一个个设置
        ___set_using(ptr + i);
}

PRIVATE inline void ___set_free(void *ptr) { //设置某个字节不被使用
    int no_of_ptr = HEAP_INFO.top - ptr;
    byte origin = get_buffer_byte(HEAP_INFO.mem_map + (no_of_ptr >> 3));
    set_buffer_byte(HEAP_INFO.mem_map + (no_of_ptr >> 3), origin & (~(1 << (no_of_ptr & 0x7))));
}

PRIVATE void __set_free(void *ptr, int size) { //设置某段内存不被使用
    for (int i = 0 ; i < size ; i ++)
        ___set_free(ptr + i);
}

PRIVATE void *__extend_heap(int size) { //拓展堆
    if (HEAP_INFO.bottom - size < HEAP_INFO.limit) return NULL; //不可拓展
    HEAP_INFO.bottom -= size;
    return HEAP_INFO.bottom + 1;
}

PRIVATE int __lookup_entry(void *start) { //寻找与start对应的内存项
    struct mem_entry entry;
    for (int i = 0 ; i < HEAP_MAX_SIZE ; i ++) {
        cp_from_buffer(HEAP_INFO.mem_table + i * sizeof (struct mem_entry), &entry, sizeof (struct mem_entry));
        if (((void*)entry.start) == start) return i; //与其相对应
    }
    return -1; //没有对应项
}

PRIVATE void __insert_entry(struct mem_entry *entry) { //插入内存项
    int idx = __lookup_entry(NULL);
    cp_to_buffer(&entry, HEAP_INFO.mem_table + idx * sizeof (struct mem_entry), sizeof (struct mem_entry));
}

PRIVATE int __get_entry_size(void *start) { //获取项所对内存大小
    struct mem_entry entry;
    int idx = __lookup_entry(start);
    cp_from_buffer(HEAP_INFO.mem_table + idx * sizeof (struct mem_entry), &entry, sizeof (struct mem_entry));
    return entry.size;
}

PRIVATE void __delete_entry(void *start) { //删除内存项
    struct mem_entry entry = {NULL, 0};
    int idx = __lookup_entry(start);
    cp_to_buffer(&entry, HEAP_INFO.mem_table + idx * sizeof (struct mem_entry), sizeof (struct mem_entry));
}

PRIVATE void *__lookup_free_memory(int size) { //寻找可用内存
    for (void *i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) {
        if (! query_using(i, size)) {
            return i;
        }
    }
    return __extend_heap(size); //当前范围内没有, 拓展堆
}

PUBLIC void *malloc(int size) {
    void *ptr = __lookup_free_memory(size); //寻找可用内存
    if (ptr == NULL) //没有
        return NULL;
    struct mem_entry entry;
    entry.start = ptr;
    entry.size = size;
    __insert_entry(&entry); //插入内存项
    __set_using(ptr, size); //标记使用
    return ptr;
}

PUBLIC void *calloc(int num, int size) {
    return malloc(num * size);
}

PUBLIC void free(void *ptr) {
    if (ptr < HEAP_INFO.bottom || ptr > HEAP_INFO.top)
        return; //不在范围内 忽略
    int size = __get_entry_size(ptr); //获取内存大小
    __set_free(ptr, size);
    __delete_entry(ptr);
}

PUBLIC void set_heap_top(void *top) {
    HEAP_INFO.top = top;
}

PUBLIC void set_heap_limit(void *limit) {
    HEAP_INFO.limit = limit;
}

PUBLIC int get_used(void) {
    int num = 0;
    for (void *i = HEAP_INFO.top ; i > HEAP_INFO.bottom ; i --) { //一个个查
        num += __query_using(i);
    }
    return num;
}

PUBLIC int get_remain(void) {
    return HEAP_INFO.top - HEAP_INFO.limit - get_used(); //堆全部的内存 - 已用的
}