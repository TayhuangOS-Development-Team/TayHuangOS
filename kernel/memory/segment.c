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
 * segment.c
 * 
 * 内存段
 * 
 */



#include "segment.h"
#include <tayhuang/io.h>
#include <tayhuang/descs.h>

PRIVATE struct desc_struct GDT[8];
PRIVATE struct gdt_ptr gdtr;

PUBLIC void init_gdt(void) {
    int cs_idx = rdcs() >> 3;
    int ds_idx = rdds() >> 3;
    int tr_idx = 3;
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[ds_idx] = (struct desc_struct)GDT_ENTRY(0xA093, 0, 0xFFFFF);
    GDT[tr_idx] = (struct desc_struct)GDT_ENTRY(0, 0, 0); //WIP
    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT);
    asmv ("lgdt %0" : : "m"(gdtr));
}