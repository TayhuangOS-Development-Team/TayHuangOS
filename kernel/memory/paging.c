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

#include <memory/kheap.h>

#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>
#include <logging.h>
#include <printk.h>
#include <string.h>

PUBLIC void *create_pgd(void) { //获取页目录
    void *pml4pg = alloc_pages(0);
    memset(pml4pg, 0, 4096);
    return pml4pg;
}

PRIVATE void *__get_free_page(void) { //获取空闲页
    void *page = alloc_pages(0);
    memset(page, 0, 4096);
    return page;
}

PRIVATE bool __set_mapping(void *__pgd, void *from, void *to, bool rw, bool us) {
    int pml4e_idx = (((qword)from) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)from) >> 30) & 0x1FF;
    int pde_idx = (((qword)from) >> 21) & 0x1FF;
    int pte_idx = (((qword)from) >> 12) & 0x1FF;

    PML4E *pml4 = (PML4E*)__pgd;
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

PUBLIC bool set_mapping(void *pgd, void *from, void *to, int pages, bool rw, bool us) { //设置映射
    for (int i = 0 ; i < pages ; i ++) {
        if (! __set_mapping(pgd, from + i * MEMUNIT_SZ, to + i * MEMUNIT_SZ, rw, us)) {
            return false;
        }
    }
    return true;
}

//获取物理地址
PUBLIC void *get_physical_address(void *__pgd, void *vaddr) {
    int pml4e_idx = (((qword)vaddr) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)vaddr) >> 30) & 0x1FF;
    int pde_idx = (((qword)vaddr) >> 21) & 0x1FF;
    int pte_idx = (((qword)vaddr) >> 12) & 0x1FF;
    int offset = ((qword)vaddr) & 0xFFF;

    PML4E *pml4 = __pgd;
    if (! pml4[pml4e_idx].p) { //不存在
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PML4 %p, Index %#03X", pml4, pml4e_idx);
        //lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    PDPTE *pdpt = pml4[pml4e_idx].address << 12;
    if (! pdpt[pdpte_idx].p) { //不存在
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PDPT %p, Index %#03X", pdpt, pdpte_idx);
        //lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    PDE *pd = pdpt[pdpte_idx].address << 12;
    if (! pd[pde_idx].p) { //不存在
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PD %p, Index %#03X", pd, pde_idx);
        //lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }
    
    PTE *pt = pd[pde_idx].address << 12;
    if (! pt[pte_idx].p) { //不存在
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PT %p, Index %#03X", pt, pte_idx);
        //lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    void *paddr = (void*)((pt[pte_idx].address << 12) + offset);
    return paddr;
}