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

    
}

void set_mapping(_IN void *from, _IN void *to) {

}