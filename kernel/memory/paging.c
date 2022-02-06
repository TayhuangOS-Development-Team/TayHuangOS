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

#define PAGING1_ADDRESS (0x320000)
#define PAGING1_LIMIT_LOWEST (0x5A0000)
#define PAGING1_LIMIT_LOW (0x720000)
#define PAGING1_LIMIT_MID (0xA20000)
#define PAGING1_LIMIT_HIGH (0x1320000)
#define PAGING1_LIMIT_HIGHEST (0x1B20000)

PRIVATE int paging1_limit = PAGING1_LIMIT_LOW;

void init_paging(_IN int pml4_num, _IN int pdpt_num, _IN int pd_num, _IN int pt_num) {

}

void set_mapping(_IN void *from, _IN void *to) {

}