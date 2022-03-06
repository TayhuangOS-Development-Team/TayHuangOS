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
#include "../kheap.h"
#include <tayhuang/paging.h>
#include <tayhuang/control_registers.h>

PRIVATE const int pte_pmu = (MEMUNIT_SZ / sizeof(PTE)); //PTE Per MEMUNIT
PRIVATE const int pde_pmu = (MEMUNIT_SZ / sizeof(PDE)); //PDE Per MEMUNIT
PRIVATE const int pdpte_pmu = (MEMUNIT_SZ / sizeof(PDPTE)); //PDPTE Per MEMUNIT
PRIVATE const int pml4e_pmu = (MEMUNIT_SZ / sizeof(PML4E)); //PML4E Per MEMUNIT

PUBLIC void *init_paging(_IN qword _vmemsz, _IN qword paging1_start, _OUT qword *pagingsz) {
    const int page_num = _vmemsz / MEMUNIT_SZ; //页数
    const int pt_num = (page_num / pte_pmu + ((page_num % pte_pmu) != 0)) * 3 / 2; //PT数
    const int pd_num = max(16, ((pt_num / pde_pmu + ((pt_num % pde_pmu) != 0))) * 3 / 2); //PD数
    const int pdpt_num = max(16, (pd_num / pdpte_pmu + ((pd_num % pdpte_pmu) != 0)) * 3 / 2); //PDPT数
    const int pml4_num = (pdpt_num / pml4e_pmu + ((pdpt_num % pml4e_pmu) != 0)); //PML4数
    if (pml4_num != 1)
        return NULL;
    return _init_paging(paging1_start, pml4_num, pdpt_num, pd_num, pt_num, pagingsz); //初始化分页
}

PRIVATE int paging1_limit = NULL;
PRIVATE PML4E *pml4_start_address = NULL;
PRIVATE void *pdpt_start_address = NULL;
PRIVATE void *pd_start_address = NULL;
PRIVATE void *pt_start_address = NULL;
PRIVATE int _pml4_num = 0;
PRIVATE int _pdpt_num = 0;
PRIVATE int _pd_num = 0;
PRIVATE int _pt_num = 0;

PUBLIC void *_init_paging(_IN qword paging1_start, _IN int pml4_num, _IN int pdpt_num, _IN int pd_num, _IN int pt_num, _OUT qword *pagingsz) {
    qword paging_sz = (pml4_num + pdpt_num + pd_num + pt_num) * MEMUNIT_SZ;
    *pagingsz = paging_sz;

    paging1_limit = paging1_start + *pagingsz;

    _pml4_num = pml4_num;
    _pdpt_num = pdpt_num;
    _pd_num = pd_num;
    _pt_num = pt_num;

    pml4_start_address = paging1_start;
    pdpt_start_address = ((void*)pml4_start_address) + pml4_num * MEMUNIT_SZ;
    pd_start_address = pdpt_start_address + pdpt_num * MEMUNIT_SZ;
    pt_start_address = pd_start_address + pd_num * MEMUNIT_SZ;

    memset (pml4_start_address, 0, pml4_num * MEMUNIT_SZ); //设0
    memset (pdpt_start_address, 0, pdpt_num * MEMUNIT_SZ); //设0
    memset (pd_start_address, 0, pd_num * MEMUNIT_SZ); //设0
    memset (pt_start_address, 0, pt_num * MEMUNIT_SZ); //设0

    return pml4_start_address;
}

PRIVATE PTE *__get_free_pt(void) {
    for (int i = 0 ; i < _pt_num ; i ++) {
        bool flag = false;
        qword *pt = (qword*)(pt_start_address + i * MEMUNIT_SZ);
        for (int j = 0 ; j < pte_pmu ; j ++) {
            if (pt[j] != 0) {
                flag = true;
            }
        }
        if (! flag) {
            return (PTE*)pt;
        }
    }
    return NULL;
}

PRIVATE PDE *__get_free_pd(void) {
    for (int i = 0 ; i < _pd_num ; i ++) {
        bool flag = false;
        qword *pd = (qword*)(pd_start_address + i * MEMUNIT_SZ);
        for (int j = 0 ; j < pde_pmu ; j ++) {
            if (pd[j] != 0) {
                flag = true;
            }
        }
        if (! flag) {
            return (PDE*)pd;
        }
    }
    return NULL;
}

PRIVATE PDPTE *__get_free_pdpt(void) {
    for (int i = 0 ; i < _pdpt_num ; i ++) {
        bool flag = false;
        qword *pdpt = (qword*)(pdpt_start_address + i * MEMUNIT_SZ);
        for (int j = 0 ; j < pdpte_pmu ; j ++) {
            if (pdpt[j] != 0) {
                flag = true;
            }
        }
        if (! flag) {
            return (PDPTE*)pdpt;
        }
    }
    return NULL;
}

PRIVATE bool __set_mapping(_IN void *from, _IN void *to, _IN bool rw, _IN bool us) {
    int pml4e_idx = (((qword)from) >> 39) & 0x1FF;
    int pdpte_idx = (((qword)from) >> 30) & 0x1FF;
    int pde_idx = (((qword)from) >> 21) & 0x1FF;
    int pte_idx = (((qword)from) >> 12) & 0x1FF;

    PML4E *pml4 = pml4_start_address;
    if (pml4[pml4e_idx].p == false) { //当前PML4E未分配
        pml4[pml4e_idx].p = true;
        pml4[pml4e_idx].rw = false;
        pml4[pml4e_idx].us = false;
        pml4[pml4e_idx].pcd = false;
        pml4[pml4e_idx].pwt = false;
        pml4[pml4e_idx].a = false;
        pml4[pml4e_idx].address = (((qword)__get_free_pdpt()) >> 12);
    }

    PDPTE *pdpt = (PDPTE*)(pml4[pml4e_idx].address << 12);
    if (pdpt == NULL) return false;
    if (pdpt[pdpte_idx].p == false) { //当前PDPTE未分配
        pdpt[pdpte_idx].p = true;
        pdpt[pdpte_idx].rw = false;
        pdpt[pdpte_idx].us = false;
        pdpt[pdpte_idx].pcd = false;
        pdpt[pdpte_idx].pwt = false;
        pdpt[pdpte_idx].a = false;
        pdpt[pdpte_idx].address = (((qword)__get_free_pd()) >> 12);
    }

    PDE *pd = (PDE*)(pdpt[pdpte_idx].address << 12);
    if (pd == NULL) return false;
    if (pd[pde_idx].p == false) { //当前PDE未分配
        pd[pde_idx].p = true;
        pd[pde_idx].rw = false;
        pd[pde_idx].us = false;
        pd[pde_idx].pcd = false;
        pd[pde_idx].pwt = false;
        pd[pde_idx].a = false;
        pd[pde_idx].address = (((qword)__get_free_pt()) >> 12);
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

PUBLIC bool set_mapping(_IN void *from, _IN void *to, _IN int pages, _IN bool rw, _IN bool us) {
    for (int i = 0 ; i < pages ; i ++) {
        if (! __set_mapping(from + i * MEMUNIT_SZ, to + i * MEMUNIT_SZ, rw, us)) {
            return false;
        }
    }
    return true;
}