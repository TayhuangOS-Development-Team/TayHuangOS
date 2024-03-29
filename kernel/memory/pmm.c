/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
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
#include <memory/paging.h>

#include <string.h>

#include <logging.h>

typedef struct __free_area {
    void *address;
    struct __free_area *next;
} free_area_struct;

//最多一次性2^9个页面 2MB
#define MAX_ORDER (9) 

//编号为i的列表里的内存其大小为2^i个内存页
free_area_struct *list_head[MAX_ORDER + 1]; 
free_area_struct *list_tail[MAX_ORDER + 1];

//增加空闲区域
PRIVATE void add_free_area(int order, void *address) { 
    free_area_struct *head = list_head[order];
    
    //头为空
    if (head == NULL) { 
        head = kmalloc(sizeof(free_area_struct));
        head->address = address;
        head->next = NULL;
        list_tail[order] = list_head[order] = head;
        return;
    }

    //加到队尾
    free_area_struct *tail = list_tail[order];
    list_tail[order] = kmalloc(sizeof(free_area_struct));
    tail->next = list_tail[order];
    list_tail[order]->address = address;
    list_tail[order]->next = NULL;
}

//初始化pmm
PUBLIC void init_pmm(size_t memsize, void *reserved_limit) {
    qword pages = ((memsize + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1)) / MEMUNIT_SZ;
    linfo ("PMM", "memsize = %#016X, %#016X pages in total", memsize, pages);
    qword reserved_pages = ((((size_t)reserved_limit) + (MEMUNIT_SZ - 1)) & ~(MEMUNIT_SZ - 1)) / MEMUNIT_SZ;
    linfo ("PMM", "%#016X pages added", pages - reserved_pages);
    pages -= reserved_pages;

    void *start = reserved_limit;

    //用二进制拆分添加页面
    for (int i = 0 ; i < MAX_ORDER ; i ++) {
        if ((pages & 1) != 0) {
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

//分配空闲内存 最低2^order个内存页
PUBLIC void *__alloc_free_pages(int order, int *order_give) {
    for (int i = order ; i <= MAX_ORDER ; i ++) {
        free_area_struct *head = list_head[i];

        //可用
        if (head != NULL) {
            list_head[i] = head->next;
            if (head->next == NULL)
                list_tail[i] = NULL;
            *order_give = i;
            return head->address;
        }
    }
    return NULL;
}

PUBLIC void *alloc_pages(int order) {
    int order_give = 0;
    // 寻找可用页
    void *addr = __alloc_free_pages(order, &order_give);

    //返还多余页
    return_pages(addr + (MEMUNIT_SZ << order), (1 << order_give) - (1 << order));

    return addr;
}

//归还内存
PUBLIC void __return_pages(void *addr, int order) {
    add_free_area(order, addr);
}

PUBLIC void return_pages(void *addr, int pages) {
    void *start = addr;

    //用二进制拆分归还页面
    for (int i = 0 ; i < MAX_ORDER ; i ++) {
        if ((pages & 1) != 0) {
            __return_pages(start, i);
            start += MEMUNIT_SZ << i;
        }
        pages >>= 1;
    }

    for (int i = 0 ; i < pages ; i ++) {
        __return_pages(start, MAX_ORDER);
        start += MEMUNIT_SZ << MAX_ORDER;
    }
}

//给给定页表分配逻辑上连续的内存
PUBLIC void alloc_vpages(void *pgd, void *addr, int pages) {
    //用二进制拆分分配页面
    void *vaddr = addr;
    for (int i = 0 ; i < MAX_ORDER ; i ++) {
        if ((pages & 1) != 0) {
            void *paddr = alloc_pages(i);
            set_mapping(pgd, vaddr, paddr, 1 << i, true, true);
            vaddr += (MEMUNIT_SZ << i);
        }
        pages >>= 1;
    }
    
    for (int i = 0 ; i < pages ; i ++) {
        void *paddr = alloc_pages(MAX_ORDER);
        set_mapping(pgd, vaddr, paddr, 1 << MAX_ORDER, true, true);
        vaddr += (MEMUNIT_SZ << MAX_ORDER);
    }
}

PUBLIC void vmemset(void *pgd, void *addr, int val, size_t size) {
    //利用for逐字节设置
    for (int i = 0 ; i < size ; i ++) {
        *(byte *)__pa(pgd, addr) = val;
        addr ++;
    }
}

PUBLIC void vpmemcpy(void *dst, void *src_pgd, void *src, size_t size) {
    //利用for逐字节复制
    for (int i = 0 ; i < size ; i ++) {
        *(byte *)dst = *(byte *)__pa(src_pgd, src);
        dst ++;
        src ++;
    }
}

PUBLIC void pvmemcpy(void *dst_pgd, void *dst, void *src, size_t size) {
    //利用for逐字节复制
    for (int i = 0 ; i < size ; i ++) {
        *(byte *)__pa(dst_pgd, dst) = *(byte *)src;
        dst ++;
        src ++;
    }
}

PUBLIC void vvmemcpy(void *dst_pgd, void *dst, void *src_pgd, void *src, size_t size) {
    //利用for逐字节复制
    for (int i = 0 ; i < size ; i ++) {
        *(byte *)__pa(dst_pgd, dst) = *(byte *)__pa(src_pgd, src);
        dst ++;
        src ++;
    }
}