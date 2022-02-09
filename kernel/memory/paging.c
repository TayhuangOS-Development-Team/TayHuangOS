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
 * paging.c
 * 
 * 页表
 * 
 */



#include "paging.h"
#include "segment.h"
#include "../display/printk.h"
#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>

#define PAGING1_ADDRESS (0x320000)
#define PAGING1_LIMIT_LOWEST (0x5A0000)
#define PAGING1_LIMIT_LOW (0x720000)
#define PAGING1_LIMIT_MID (0xA20000)
#define PAGING1_LIMIT_HIGH (0x1320000)
#define PAGING1_LIMIT_HIGHEST (0x1B20000)

PRIVATE int paging1_limit = PAGING1_LIMIT_LOW;

void init_paging(_IN int pml4_num, _IN int pdpt_num, _IN int pd_num, _IN int pt_num, _OUT SEGMENT_TOKEN *PAGING_TOKEN) {
    long long paging_sz = (pml4_num + pdpt_num + pd_num + pt_num) * MEMUNIT_SZ;
    if (paging_sz < (PAGING1_LIMIT_LOWEST - PAGING1_ADDRESS)) {
        paging1_limit = PAGING1_LIMIT_LOWEST;
    }
    else if (paging_sz < (PAGING1_LIMIT_LOW - PAGING1_ADDRESS)) {
        paging1_limit = PAGING1_LIMIT_LOW;
    }
    else if (paging_sz < (PAGING1_LIMIT_MID - PAGING1_ADDRESS)) {
        paging1_limit = PAGING1_LIMIT_MID;
    }
    else if (paging_sz < (PAGING1_LIMIT_HIGH - PAGING1_ADDRESS)) {
        paging1_limit = PAGING1_LIMIT_HIGH;
    }
    else if (paging_sz < (PAGING1_LIMIT_HIGHEST - PAGING1_ADDRESS)) {
        paging1_limit = PAGING1_LIMIT_HIGHEST;
    }
    
    *PAGING_TOKEN = add_segment(PAGING1_ADDRESS, paging1_limit, MST_PROTECT | MST_PAGE | MST_DATA);

    const int pte_pmu = (MEMUNIT_SZ / sizeof(PTE)); //PTE Per MEMUNIT
    const int pde_pmu = (MEMUNIT_SZ / sizeof(PDE)); //PDE Per MEMUNIT
    const int pdpte_pmu = (MEMUNIT_SZ / sizeof(PDPTE)); //PDPTE Per MEMUNIT
    const int pml4e_pmu = (MEMUNIT_SZ / sizeof(PML4E)); //PML4E Per MEMUNIT

    printk ("paging size: %#16X ; paging base: %#16X ; paging limit: %#16X\n", paging_sz, PAGING1_ADDRESS, paging1_limit);
    printk ("pmu: %d\n", pte_pmu);

    PML4E* const pml4_start_address = PAGING1_ADDRESS;
    PDPTE* const pdpt_start_address = ((void*)pml4_start_address) + pml4_num * MEMUNIT_SZ;
    PDE* const pd_start_address = ((void*)pdpt_start_address) + pdpt_num * MEMUNIT_SZ;
    PTE* const pt_start_address = ((void*)pd_start_address) + pd_num * MEMUNIT_SZ;
    printk ("pml4 start: %p ; ", pml4_start_address);
    printk ("pdpt start: %p ;\n", pdpt_start_address);
    printk ("pd start: %p ; ", pd_start_address);
    printk ("pt start: %p ;\n", pt_start_address);

    //初始化PT
    for (int i = 0 ; i < pt_num * pte_pmu; i ++) {
        PTE *pte = pt_start_address + i;
        pte->reserved = 0;
        pte->p = true;
        pte->rw = true;
        pte->us = false;
        pte->pwt = false;
        pte->pcd = false;
        pte->a = false;
        pte->d = false;
        pte->pat = false;
        pte->g = false;
        pte->xd = false;
        pte->address = i;
    }
    //初始化PD
    for (int i = 0 ; i < pd_num * pde_pmu; i ++) {
        PDE *pde = pd_start_address + i;
        pde->reserved = 0;
        pde->reserved2 = 0;
        pde->p = true;
        pde->rw = true;
        pde->us = false;
        pde->pwt = false;
        pde->pcd = false;
        pde->a = false;
        pde->xd = false;
        pde->address = (((qword)pt_start_address) >> 12) + i;
    }
    //初始化PDPT
    for (int i = 0 ; i < pdpt_num * pdpte_pmu ; i ++) {
        PDPTE *pdpte = pdpt_start_address + i;
        pdpte->reserved = 0;
        pdpte->reserved2 = 0;
        pdpte->p = true;
        pdpte->rw = true;
        pdpte->us = false;
        pdpte->pwt = false;
        pdpte->pcd = false;
        pdpte->a = false;
        pdpte->xd = false;
        pdpte->address = ((((qword)pd_start_address)) >> 12) + i;
    }
    //初始化PML4
    for (int i = 0 ; i < pml4_num * pml4e_pmu ; i ++) {
        PML4E *pml4e = pml4_start_address + i;
        pml4e->reserved = 0;
        pml4e->reserved2 = 0;
        pml4e->p = true;
        pml4e->rw = true;
        pml4e->us = false;
        pml4e->pwt = false;
        pml4e->pcd = false;
        pml4e->a = false;
        pml4e->xd = false;
        pml4e->address = ((((qword)pdpt_start_address)) >> 12) + i;
    }
    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)pml4_start_address;
    asmv ("xchg %bx, %bx");
    set_cr3(cr3);
}

void set_mapping(_IN void *from, _IN void *to) {

}