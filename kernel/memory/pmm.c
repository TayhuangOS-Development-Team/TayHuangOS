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



#include <memory/pmm.h>
#include <tayhuang/defs.h>
#include <tayhuang/paging.h>
#include <tayhuang/partition.h>
#include <memory/kheap.h>
#include <string.h>

PRIVATE qword totalmem_sum = 0;
PRIVATE qword *memstates = NULL;

PUBLIC void init_pmm(qword totalmem) {
    totalmem_sum = totalmem;
    memstates = kmalloc(totalmem / MEMUNIT_SZ / 8);
}

// PRIVATE bool __get_page_state(void *page) {
//     qword page_no = ((qword)page) / MEMUNIT_SZ;
//     qword page_idx = page_no / sizeof(qword);
//     qword page_bit = page_no % sizeof(qword);

//     qword states = memstates[page_idx];
//     return (states & (1 << page_bit)) != 0;
// }

PRIVATE void __set_page_state(void *page, bool state) {
    qword page_no = ((qword)page) / MEMUNIT_SZ;
    qword page_idx = page_no / sizeof(qword);
    qword page_bit = page_no % sizeof(qword);

    memstates[page_idx] |= (1 << page_bit);
}

PUBLIC qword get_freemem_sum(void){
    //TODO
    return 0;
}

PUBLIC qword get_usedmem_sum(void){
    //TODO
    return 0;
}

PUBLIC qword get_totalmem_sum(void){
    return totalmem_sum;
}

//设置从addr开始的pages个page状态
PUBLIC void __set_pages_state(void *addr, qword pages, bool used){
    for (qword i = 0 ; i < pages ; i ++) {
        __set_page_state(addr + i * MEMUNIT_SZ, used);
    }
}

PRIVATE qword __get_free_pages(void *addr, qword maxsum) {
    qword page_no = ((qword)addr) / MEMUNIT_SZ;
    qword page_idx = page_no / sizeof(qword);
    qword page_bit = page_no % sizeof(qword);

    qword states = memstates[page_idx];
    states >>= page_bit; //移位 除去低位

    qword sum = 0;

    if (states == 0) {
        sum += sizeof(qword) * 8 - page_bit;
    }

    if (maxsum != -1 && sum >= maxsum)
        return maxsum;

    qword i = 0;
    for (i = page_idx + 1 ; memstates[i] == 0 ; i ++) {
        sum += sizeof(qword) * 8;
        if (maxsum != -1 && sum >= maxsum)
            return maxsum;
    }

    sum += simple_log2(LOWBIT(memstates[i]));
    return min(sum, maxsum);
}

//从addr开始的pages个page是否全部空闲
PUBLIC bool __is_pages_all_free(void *addr, qword pages){
    qword page_no = ((qword)addr) / MEMUNIT_SZ;
    qword page_idx = page_no / sizeof(qword);
    qword page_bit = page_no % sizeof(qword);

    qword states = memstates[page_idx];
    states >>= page_bit; //移位 除去低位

    qword sum = 0;

    if (states == 0) {
        sum += sizeof(qword) * 8 - page_bit;
    }

    if (sum >= pages)
        return true;

    qword i = 0;
    for (i = page_idx + 1 ; memstates[i] == 0 ; i ++) {
        sum += sizeof(qword) * 8;
        if (sum >= pages)
            return true;
    }

    sum += simple_log2(LOWBIT(memstates[i]));
    if (sum >= pages)
        return true;

    return false;
}


//获取pages个连续页（强制性）
PUBLIC void *__find_continuous_pages(qword pages) {
    //TODO
    qword pos = 0;
    for (qword free_pages = __get_free_pages(pos * MEMUNIT_SZ, pages) ;
        free_pages < pages ;
    ) {
        pos += free_pages;
    }
    __set_pages_state((void*)(pos * MEMUNIT_SZ), pages, true);
    return pos * MEMUNIT_SZ;
}
