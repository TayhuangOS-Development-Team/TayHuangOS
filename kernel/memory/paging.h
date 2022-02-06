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
 * paging.h
 * 
 * 页表
 *
 */



void init_paging(int pml4_num, int pdpt_num, int pd_num, int pt_num);
void set_mapping(void *from, void *to);