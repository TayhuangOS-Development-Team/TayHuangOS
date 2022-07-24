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
 * paging.c
 *
 * 页表
 *
 */



#include <memory/paging.h>
#include <memory/pmm.h>

#include <memory/kheap.h>

#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>

#include <task/task_scheduler.h>

#include <logging.h>
#include <printk.h>
#include <string.h>

//获取页目录
PUBLIC void *create_pgd(void) { 
    void *pml4pg = alloc_pages(0);
    memset(pml4pg, 0, 4096);
    return pml4pg;
}

//获取空闲页
PRIVATE void *__get_free_page(void) { 
    void *page = alloc_pages(0);
    memset(page, 0, 4096);
    return page;
}

PRIVATE bool __set_mapping(void *__pgd, void *vaddr, void *paddr, bool rw, bool us) {
    //获取各级页项索引
    int pml4e_idx = (((qword)vaddr) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)vaddr) >> 30) & 0x1FF;
    int pde_idx = (((qword)vaddr) >> 21) & 0x1FF;
    int pte_idx = (((qword)vaddr) >> 12) & 0x1FF;

    PML4E *pml4 = (PML4E*)__pgd;
    //当前PML4E未分配
    if (pml4[pml4e_idx].p == false) { 
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
    //当前PDPTE未分配
    if (pdpt[pdpte_idx].p == false) {
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
    //当前PDE未分配
    if (pd[pde_idx].p == false) { 
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
    //当前PTE未分配
    if (pt[pte_idx].p == false) { 
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
    pt[pte_idx].address = (((qword)paddr) >> 12);
   
    return true;
}

//设置映射
PUBLIC bool set_mapping(void *pgd, void *vaddr, void *paddr, int pages, bool rw, bool us) {
    //对齐
    vaddr = (void*)((qword)(vaddr) & ~(MEMUNIT_SZ - 1));
    for (int i = 0 ; i < pages ; i ++) {
        if (! __set_mapping(pgd, vaddr + (i * MEMUNIT_SZ), paddr + (i * MEMUNIT_SZ), rw, us)) {
            return false;
        }
    }
    return true;
}

PUBLIC bool set_mappingvv(void *src_pgd, void *src_addr, void *dst_pgd, void *dst_addr, int pages, bool rw, bool us) {
    //对齐
    src_addr = (void*)((qword)(src_addr) & ~(MEMUNIT_SZ - 1));
    dst_addr = (void*)((qword)(dst_addr) & ~(MEMUNIT_SZ - 1));
    for (int i = 0 ; i < pages ; i ++) {
        if (! __set_mapping(dst_pgd, dst_addr + (i * MEMUNIT_SZ), __pa(src_pgd, src_addr + (i * MEMUNIT_SZ)), rw, us)) {
            return false;
        }
    }
    return true;
}

//获取物理地址
PUBLIC void *get_physical_address(void *__pgd, void *vaddr) {
    qword addr = (qword)vaddr;
    qword offset = addr & 0xFFF; addr >>= 12;
    qword pte_idx = addr & 0x1FF; addr >>= 9;
    qword pde_idx = addr & 0x1FF; addr >>= 9;
    qword pdpte_idx = addr & 0x1FF; addr >>= 9;
    qword pml4e_idx = addr & 0x1FF; addr >>= 9;

    PML4E *pml4 = __pgd;
    //不存在
    if (pml4[pml4e_idx].p == false) { 
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PML4 %p, Index %#03X", pml4, pml4e_idx);
        lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    PDPTE *pdpt = pml4[pml4e_idx].address << 12;
    //不存在
    if (pdpt[pdpte_idx].p == false) {
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PML4 %p PDPT %p, Index %#03X", pml4, pdpt, pdpte_idx);
        lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    PDE *pd = pdpt[pdpte_idx].address << 12;
    //不存在
    if (pd[pde_idx].p == false) {
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PML4 %p PDPT %p PD %p, Index %#03X", pml4, pdpt, pd, pde_idx);
        lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    PTE *pt = pd[pde_idx].address << 12;
    //不存在
    if (pt[pte_idx].p == false) {
        lerror ("Paging", "Address %p doesn't exist!", vaddr);
        lerror ("Paging", "Error In PML4 %p PDPT %p PD %p PT %p, Index %#03X", pml4, pdpt, pd, pt, pte_idx);
        lerror ("Paging", "Current Task(PID = %d)", current_task->pid);
        return NULL;
    }

    //获取地址, 加上offse
    void *paddr = (void*)((pt[pte_idx].address << 12) + offset);
    return paddr;
}