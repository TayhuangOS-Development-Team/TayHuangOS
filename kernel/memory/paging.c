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
#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>

PUBLIC void init_paging(_IN qword _vmemsz, _OUT SEGMENT_TOKEN *PAGING_TOKEN) {
    const int page_num = _vmemsz / MEMUNIT_SZ;
    const int pt_num = page_num / (MEMUNIT_SZ / sizeof(PTE));
    const int pd_num = page_num / (MEMUNIT_SZ / sizeof(PDE));
    const int pdpt_num = page_num / (MEMUNIT_SZ / sizeof(PDPTE));
    const int pml4_num = page_num / (MEMUNIT_SZ / sizeof(PML4E));
    _init_paging(pml4_num, pdpt_num, pd_num, pt_num, PAGING_TOKEN);
}

#define PAGING1_ADDRESS (0x320000)
#define PAGING1_LIMIT_LOWEST (0x5A0000)
#define PAGING1_LIMIT_LOW (0x720000)
#define PAGING1_LIMIT_MID (0xA20000)
#define PAGING1_LIMIT_HIGH (0x1320000)
#define PAGING1_LIMIT_HIGHEST (0x1B20000)

PRIVATE int paging1_limit = PAGING1_LIMIT_LOW;
PRIVATE PML4E *pml4_start_address = NULL;

PUBLIC void _init_paging(_IN int pml4_num, _IN int pdpt_num, _IN int pd_num, _IN int pt_num, _OUT SEGMENT_TOKEN *PAGING_TOKEN) {
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

    pml4_start_address = PAGING1_ADDRESS;
    PDPTE *const pdpt_start_address = ((void*)pml4_start_address) + pml4_num * MEMUNIT_SZ;
    PDE *const pd_start_address = ((void*)pdpt_start_address) + pdpt_num * MEMUNIT_SZ;
    PTE *const pt_start_address = ((void*)pd_start_address) + pd_num * MEMUNIT_SZ;

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
        pde->address = (((qword)pt_start_address) / MEMUNIT_SZ) + i;
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
        pdpte->address = ((((qword)pd_start_address)) / MEMUNIT_SZ) + i;
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
        pml4e->address = ((((qword)pdpt_start_address)) / MEMUNIT_SZ) + i;
    }
    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)pml4_start_address;
    set_cr3(cr3);
}

PRIVATE void __set_mapping(_IN void *from, _IN void *to) { 
    PML4E *pml4e = pml4_start_address + ((((qword)from) >> 39) & 0x1FF);
    PDPTE *pdpte = ((PDPTE*)(pml4e->address * MEMUNIT_SZ)) + ((((qword)from) >> 30) & 0x1FF);
    PDE *pde = ((PDE*)(pdpte->address * MEMUNIT_SZ)) + ((((qword)from) >> 21) & 0x1FF);
    PTE *pte = ((PTE*)(pde->address * MEMUNIT_SZ)) + ((((qword)from) >> 12) & 0x1FF);
    pte->address = ((qword)to) / MEMUNIT_SZ;
}

PUBLIC void set_mapping(_IN void *from, _IN void *to, _IN int pages) {
    for (int i = 0 ; i < pages ; i ++) {
        __set_mapping(from + i * MEMUNIT_SZ, to + i * MEMUNIT_SZ);
    }
}