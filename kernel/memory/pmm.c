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
 * 物理内存管理
 * 
 */



#include <tayhuang/defs.h>
#include <tayhuang/paging.h>
#include <tayhuang/partition.h>

#include <memory/pmm.h>
#include <memory/kheap.h>

#include <string.h>

#include <logging.h>

typedef struct __free_area {
    void *address;
    struct __free_area *next;
} free_area_struct; //16B

#define MAX_ORDER (9) //最多一次性2^9个页面 2MB

free_area_struct *list_head[MAX_ORDER + 1]; 
free_area_struct *list_tail[MAX_ORDER + 1];

PRIVATE void add_free_area(int order, void *address) {
    free_area_struct *head = list_head[order];
    
    if (head == NULL) {
        head = kmalloc(sizeof(free_area_struct));
        head->address = address;
        head->next = NULL;
        list_tail[order] = list_head[order] = head;
        return;
    }

    free_area_struct *tail = list_tail[order];
    list_tail[order] = kmalloc(sizeof(free_area_struct));
    tail->next = list_tail[order];
    list_tail[order]->address = address;
    list_tail[order]->next = NULL;
}

PUBLIC void init_pmm(qword memsize, void *reserved_limit) {
    qword pages = ((memsize + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1)) / MEMUNIT_SZ;
    linfo ("PMM", "memsize = %#016X, %#016X pages in total", memsize, pages);
    qword reserved_pages = ((((qword)reserved_limit) + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1)) / MEMUNIT_SZ;
    linfo ("PMM", "%#016X pages need added", pages - reserved_pages);
    pages -= reserved_pages;

    void *start = reserved_limit;

    for (int i = 0 ; i < MAX_ORDER ; i ++) {
        if (pages & 1) {
            add_free_area(i, start);
            start += MEMUNIT_SZ << 1;
            linfo ("PMM", "%P ~ %P added", start, start + (MEMUNIT_SZ << i) - 1);
        }
        pages >>= 1;
    }

    for (int i = 0 ; i < pages ; i ++) {
        add_free_area(MAX_ORDER, start);
        linfo ("PMM", "%P ~ %P added", start, start + (MEMUNIT_SZ << MAX_ORDER) - 1);
        start += MEMUNIT_SZ << MAX_ORDER;
    }
}

PUBLIC void *__alloc_free_pages(int order, int *order_give) {
    for (int i = order ; i <= MAX_ORDER ; i ++) {
        free_area_struct *head = list_head[i];

        if (head != NULL) {
            list_head[i] = head->next;
            if (head->next == NULL)
                list_tail[i] = NULL;
            return head->address;
        }
    }
    return NULL;
}

PUBLIC void __return_pages(void *addr, int order) {
    add_free_area(order, addr);
}

PUBLIC void return_pages(void *addr, int pages) {
    void *start = addr;

    for (int i = 0 ; i < MAX_ORDER ; i ++) {
        if (pages & 1) {
            add_free_area(i, start);
            start += MEMUNIT_SZ << 1;
        }
        pages >>= 1;
    }

    for (int i = 0 ; i < pages ; i ++) {
        add_free_area(MAX_ORDER, start);
        start += MEMUNIT_SZ << MAX_ORDER;
    }
}

PUBLIC void *alloc_pages(int order) {
    int order_give = 0;
    // 寻找可用页
    void *addr = __alloc_free_pages(order, &order_give);

    //返还多余页
    return_pages(addr + (MEMUNIT_SZ << order), (1 << order_give) - (1 << order));

    return addr;
}