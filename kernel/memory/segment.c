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
#include "../kheap.h"
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

PRIVATE memory_segment *MEM_SEGMENTS;

PRIVATE qword calc_token(_IN void *base, _IN void *limit, _IN int type) {
    return ((qword)base) * ((qword)limit) * type;
}

PUBLIC SEGMENT_TOKEN add_segment(_IN void *base, _IN void *limit, _IN int type) {
    memory_segment* new_seg = malloc (sizeof(memory_segment));
    new_seg->base = base;
    new_seg->limit = limit;
    new_seg->type = type;
    new_seg->nxt = NULL;
    if (MEM_SEGMENTS == NULL) {
        MEM_SEGMENTS = new_seg;
    }
    else {
        memory_segment *seg = MEM_SEGMENTS;
        while (seg->nxt)
            seg = seg->nxt;
        seg->nxt = new_seg;
    }
    return calc_token(base, limit, type);
}

PUBLIC void delete_segment(_IN SEGMENT_TOKEN token, _IN void *base) {
    if (MEM_SEGMENTS == NULL) {
        return;
    }
    if (MEM_SEGMENTS->base == base) {
        if (calc_token(MEM_SEGMENTS->base, MEM_SEGMENTS->limit, MEM_SEGMENTS->type) == token) {
            free (MEM_SEGMENTS);
            MEM_SEGMENTS = NULL;
        }
        return;
    }
    memory_segment *seg = MEM_SEGMENTS;
    while (seg->nxt) {
        memory_segment *nxt_seg = seg->nxt;
        if (nxt_seg->base == base) {
            if (calc_token(nxt_seg->base, nxt_seg->limit, nxt_seg->type) == token) {
                seg->nxt = nxt_seg->nxt;
                free (nxt_seg);
            }
        }
    }
    return;
}

PUBLIC void query_segment(_IN SEGMENT_TOKEN token, _IN void *base, _OUT void **limit, _OUT int *type) {
    memory_segment *seg = MEM_SEGMENTS;
    while (seg) {
        if (seg->base == base) {
            if (calc_token(seg->base, seg->limit, seg->type) == token) {
                *limit = seg->limit;
                *type = seg->type;
            }
        }
    }
}

PUBLIC int query_memory_state(_IN void *start, _IN void *limit) {
    return MST_ERROR;
}

PUBLIC void *find_free_memory(_IN int size) {
    return NULL;
}