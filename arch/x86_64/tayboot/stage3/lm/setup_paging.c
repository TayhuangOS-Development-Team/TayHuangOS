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
 * setup_paging.c
 *
 * 设置页表
 *
 */



#include "setup_paging.h"
#include <tayboot/paging.h>
#include "../printf.h"
#include "../memory.h"

PUBLIC void *setup_paging(dword memsz, dword memsz_high, void** limit) {
    const int pte_pmu = (MEMUNIT_SZ / sizeof(PTE)); //PTE Per MEMUNIT
    const int pde_pmu = (MEMUNIT_SZ / sizeof(PDE)); //PDE Per MEMUNIT
    const int pdpte_pmu = (MEMUNIT_SZ / sizeof(PDPTE)); //PDPTE Per MEMUNIT
    const int pml4e_pmu = (MEMUNIT_SZ / sizeof(PML4E)); //PML4E Per MEMUNIT

    const int page_num = memsz / MEMUNIT_SZ
                        + ((memsz % MEMUNIT_SZ) ? 1 : 0) //有余数则 + 1
                        + ((memsz_high) << (32 - sizeof(dword) + leading_zeros(MEMUNIT_SZ))); //memsz_high = mem_size >> 32
                                                                                              //所以(memsz_high << 32) / MEMUNIT_SZ = mem_size << (32 - log2(MEMUNIT_SZ))
                        //最大支持8TB
    const int pte_num = page_num;
    const int pt_num = pte_num / pte_pmu
                        + ((pte_num  % pte_pmu) ? 1 : 0); //有余数则 + 1
    const int pde_num = pt_num;
    const int pd_num = pde_num / pde_pmu
                        + ((pde_num  % pde_pmu) ? 1 : 0); //有余数则 + 1
    const int pdpte_num = pd_num;
    const int pdpt_num = pdpte_num / pdpte_pmu
                        + ((pdpte_num  % pdpte_pmu) ? 1 : 0); //有余数则 + 1
    const int pml4e_num = pdpt_num;
    const int pml4_num = pml4e_num / pml4e_pmu
                        + ((pml4e_num  % pml4e_pmu) ? 1 : 0); //有余数则 + 1
    if (pml4_num != 1) { //4级分页, 支持到8TB
        printf ("Error!Too much memory!\n");
    }

    PML4E *const pml4_start_address = PAGE_ADDRESS;
    PDPTE *const pdpt_start_address = ((void*)pml4_start_address) + pml4_num * MEMUNIT_SZ;
    PDE *const pd_start_address = ((void*)pdpt_start_address) + pdpt_num * MEMUNIT_SZ;
    PTE *const pt_start_address = ((void*)pd_start_address) + pd_num * MEMUNIT_SZ;
    void *const page_start_address = ((void*)pt_start_address) + pt_num * MEMUNIT_SZ;

    *limit = page_start_address;

    //初始化PT
    for (int i = 0 ; i < pt_num ; i ++) {
        for (int j = 0 ; j < pte_pmu ; j ++) {
            int no = i * pte_pmu + j;
            PTE *pte = pt_start_address + no;
            memset (pte, 0, sizeof(PTE));
            if (no >= pte_num) {
                pte->p = false;
            }
            else {
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
                pte->address = no;
            }
        }
    }
    //初始化PD
    for (int i = 0 ; i < pd_num ; i ++) {
        for (int j = 0 ; j < pde_pmu ; j ++) {
            int no = i * pde_pmu + j;
            PDE *pde = pd_start_address + no;
            memset (pde, 0, sizeof(PDE));
            if (no >= pde_num) {
                pde->p = false;
            }
            else {
                pde->p = true;
                pde->rw = true;
                pde->us = false;
                pde->pwt = false;
                pde->pcd = false;
                pde->a = false;
                pde->xd = false;
                pde->address = (((dword)pt_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    //初始化PDPT
    for (int i = 0 ; i < pdpt_num ; i ++) {
        for (int j = 0 ; j < pdpte_pmu ; j ++) {
            int no = i * pdpte_pmu + j;
            PDPTE *pdpte = pdpt_start_address + no;
            memset (pdpte, 0, sizeof(PDPTE));
            if (no >= pdpte_num) {
                pdpte->p = false;
            }
            else {
                pdpte->p = true;
                pdpte->rw = true;
                pdpte->us = false;
                pdpte->pwt = false;
                pdpte->pcd = false;
                pdpte->a = false;
                pdpte->xd = false;
                pdpte->address = (((dword)pd_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    //初始化PML4
    for (int i = 0 ; i < pml4_num ; i ++) {
        for (int j = 0 ; j < pml4e_pmu ; j ++) {
            int no = i * pml4e_pmu + j;
            PML4E *pml4e = pml4_start_address + no;
            memset (pml4e, 0, sizeof(PML4E));
            if (no >= pml4e_num) {
                memset(pml4e, 0, sizeof(PML4E));
            }
            else {
                pml4e->p = true;
                pml4e->rw = true;
                pml4e->us = false;
                pml4e->pwt = false;
                pml4e->pcd = false;
                pml4e->a = false;
                pml4e->xd = false;
                pml4e->address = (((dword)pdpt_start_address) + no * MEMUNIT_SZ) >> 12;
            }
        }
    }
    return pml4_start_address;
}
