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
#include "pmm.h"

#include <kheap.h>

#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>
#include <debug/logging.h>
#include <display/printk.h>

// PRIVATE const int pte_pmu = (MEMUNIT_SZ / sizeof(PTE)); //PTE Per MEMUNIT
// PRIVATE const int pde_pmu = (MEMUNIT_SZ / sizeof(PDE)); //PDE Per MEMUNIT
// PRIVATE const int pdpte_pmu = (MEMUNIT_SZ / sizeof(PDPTE)); //PDPTE Per MEMUNIT
// PRIVATE const int pml4e_pmu = (MEMUNIT_SZ / sizeof(PML4E)); //PML4E Per MEMUNIT

PRIVATE PML4E *current_pml4 = NULL;

PUBLIC void set_pml4(void *pml4) {
    current_pml4 = pml4;
}

PUBLIC void *get_pml4(void) {
    return current_pml4;
}

PUBLIC void *create_pgd(void) { //获取页目录
    void *pml4pg = lookup_free_page();
    mark_used(pml4pg);
    memset(pml4pg, 0, 4096);
    return pml4pg;
}

PRIVATE void *__get_free_page(void) { //获取空闲页
    void *page = lookup_free_page();
    mark_used(page);
    memset(page, 0, 4096);
    return page;
}

PRIVATE bool __set_mapping(void *from, void *to, bool rw, bool us) {
    int pml4e_idx = (((qword)from) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)from) >> 30) & 0x1FF;
    int pde_idx = (((qword)from) >> 21) & 0x1FF;
    int pte_idx = (((qword)from) >> 12) & 0x1FF;

    PML4E *pml4 = current_pml4;
    if (pml4[pml4e_idx].p == false) { //当前PML4E未分配
        pml4[pml4e_idx].p = true;
        pml4[pml4e_idx].rw = rw;
        pml4[pml4e_idx].us = us;
        pml4[pml4e_idx].pcd = false;
        pml4[pml4e_idx].pwt = false;
        pml4[pml4e_idx].a = false;
        pml4[pml4e_idx].address = (((qword)__get_free_page()) >> 12);
    }

    PDPTE *pdpt = (PDPTE*)(pml4[pml4e_idx].address << 12);
    if (pdpt == NULL) return false;
    if (pdpt[pdpte_idx].p == false) { //当前PDPTE未分配
        pdpt[pdpte_idx].p = true;
        pdpt[pdpte_idx].rw = rw;
        pdpt[pdpte_idx].us = us;
        pdpt[pdpte_idx].pcd = false;
        pdpt[pdpte_idx].pwt = false;
        pdpt[pdpte_idx].a = false;
        pdpt[pdpte_idx].address = (((qword)__get_free_page()) >> 12);
    }

    PDE *pd = (PDE*)(pdpt[pdpte_idx].address << 12);
    if (pd == NULL) return false;
    if (pd[pde_idx].p == false) { //当前PDE未分配
        pd[pde_idx].p = true;
        pd[pde_idx].rw = rw;
        pd[pde_idx].us = us;
        pd[pde_idx].pcd = false;
        pd[pde_idx].pwt = false;
        pd[pde_idx].a = false;
        pd[pde_idx].address = (((qword)__get_free_page()) >> 12);
    }

    PTE *pt = (PTE*)(pd[pde_idx].address << 12);
    if (pt == NULL) return false;
    if (pt[pte_idx].p == false) { //当前PTE未分配
        pt[pte_idx].p = true;
        pt[pte_idx].rw = rw;
        pt[pte_idx].us = us;
        pt[pte_idx].pcd = false;
        pt[pte_idx].pwt = false;
        pt[pte_idx].a = false;
        pt[pte_idx].d = false;
        pt[pte_idx].pat = false;
        pt[pte_idx].g = false;
    }
    pt[pte_idx].address = (((qword)to) >> 12);

    return true;
}

PUBLIC bool set_mapping(void *from, void *to, int pages, bool rw, bool us) { //设置映射
    for (int i = 0 ; i < pages ; i ++) {
        if (! __set_mapping(from + i * MEMUNIT_SZ, to + i * MEMUNIT_SZ, rw, us)) {
            return false;
        }
    }
    return true;
}

//获取物理地址
PUBLIC void *get_physical_address(void *__pml4, void *vaddr) {
    char buffer[64];
    int pml4e_idx = (((qword)vaddr) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)vaddr) >> 30) & 0x1FF;
    int pde_idx = (((qword)vaddr) >> 21) & 0x1FF;
    int pte_idx = (((qword)vaddr) >> 12) & 0x1FF;
    int offset = ((qword)vaddr) & 0xFFF;

    PML4E *pml4 = __pml4;
    if (! pml4[pml4e_idx].p) {
        sprintk (buffer, "Address %P doesn't exist!", vaddr);
        lerror (buffer);
        sprintk (buffer, "Error In PML4 %P, Index %#03X", pml4, pml4e_idx);
        lerror (buffer);
        return NULL;
    }

    PDPTE *pdpt = pml4[pml4e_idx].address << 12;
    if (! pdpt[pdpte_idx].p) {
        sprintk (buffer, "Address %P doesn't exist!", vaddr);
        lerror (buffer);
        sprintk (buffer, "Error In PDPT %P, Index %#03X", pdpt, pdpte_idx);
        lerror (buffer);
        return NULL;
    }

    PDE *pd = pdpt[pdpte_idx].address << 12;
    if (! pd[pde_idx].p) {
        sprintk (buffer, "Address %P doesn't exist!", vaddr);
        lerror (buffer);
        sprintk (buffer, "Error In PD %P, Index %#03X", pd, pde_idx);
        lerror (buffer);
        return NULL;
    }
    
    PTE *pt = pd[pde_idx].address << 12;
    if (! pt[pte_idx].p) {
        sprintk (buffer, "Address %P doesn't exist!", vaddr);
        lerror (buffer);
        sprintk (buffer, "Error In PT %P, Index %#03X", pt, pte_idx);
        lerror (buffer);
        return NULL;
    }

    void *paddr = (void*)((pt[pte_idx].address << 12) + offset);
    return paddr;
}