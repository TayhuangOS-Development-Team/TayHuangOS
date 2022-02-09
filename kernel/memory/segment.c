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

PRIVATE void insert_segment(_IN void *base, _IN void *limit, _IN int type) {
    if (MEM_SEGMENTS == NULL) {
        MEM_SEGMENTS = malloc (sizeof(memory_segment));
        MEM_SEGMENTS->base = base;
        MEM_SEGMENTS->limit = limit;
        MEM_SEGMENTS->type = type;
        MEM_SEGMENTS->nxt = NULL;
        return ;
    }
    memory_segment* new_seg = malloc (sizeof(memory_segment));
    new_seg->base = base;
    new_seg->limit = limit;
    new_seg->type = type;
    new_seg->nxt = NULL;
    memory_segment *seg = MEM_SEGMENTS;
    while (seg->nxt)
        seg = seg->nxt;
    seg->nxt = new_seg;
}

PUBLIC void init_segments(_IN void *kernel_start, _IN void *kernel_limit, _OUT SEGMENT_TOKEN *KERNEL_TOKEN, _OUT SEGMENT_TOKEN *KHEAP_TOKEN) {
    insert_segment(0, 0xFFFFFFFFFFFFFFFF, MST_FREE);

    *KERNEL_TOKEN = add_segment(kernel_start, kernel_limit, MST_PROTECT | MST_USING);
    *KHEAP_TOKEN = add_segment(KHEAP_BASE, KHEAP_LIMIT, MST_PROTECT | MST_USING | MST_DATA);

    add_segment(0xA0000, 0xFFFFF, MST_PROTECT | MST_HARDWARE);
    add_segment(0xE0000000, 0xE0800000, MST_PROTECT | MST_HARDWARE);
}

PRIVATE memory_segment *found_segment_in(void *ptr) {
    memory_segment *seg = MEM_SEGMENTS;
    while (seg) {
        if ((seg->base <= ptr) && (seg->limit >= ptr))
            return seg->base;
        seg = seg->nxt;
    }
    return NULL;
}

PUBLIC SEGMENT_TOKEN add_segment(_IN void *base, _IN void *limit, _IN int type) {
    if (MEM_SEGMENTS == NULL) {
        return 0;
    }
    memory_segment* free_segment = found_segment_in(base);
    if (free_segment == NULL) {
        return 0;
    }
    if (free_segment != found_segment_in(limit)) {
        return 0;
    }

    if (free_segment->base != base) {
        insert_segment(free_segment->base, base, MST_FREE);
    }

    if (free_segment->limit != limit) {
        insert_segment(limit, free_segment->limit, MST_FREE);
    }

    insert_segment(base, limit, type);
    
    delete_segment(NULL, free_segment);

    return calc_token(base, limit, type);
}

PUBLIC void delete_segment(_IN SEGMENT_TOKEN token, _IN void *base) {
    if (MEM_SEGMENTS == NULL) {
        return;
    }
    if (MEM_SEGMENTS->base == base) {
        if ((calc_token(MEM_SEGMENTS->base, MEM_SEGMENTS->limit, MEM_SEGMENTS->type) == token) || ((MEM_SEGMENTS->type & MST_PROTECT) == 0)) {
            memory_segment *nxt = MEM_SEGMENTS->nxt;
            free (MEM_SEGMENTS);
            MEM_SEGMENTS = nxt;
        }
        return;
    }
    memory_segment *seg = MEM_SEGMENTS;
    while (seg->nxt) {
        memory_segment *nxt_seg = seg->nxt;
        if (nxt_seg->base == base) {
            if ((calc_token(nxt_seg->base, nxt_seg->limit, nxt_seg->type) == token) || ((seg->type & MST_PROTECT) == 0)) {
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
            if ((calc_token(seg->base, seg->limit, seg->type) == token) || ((seg->type & MST_PROTECT) == 0)) {
                *limit = seg->limit;
                *type = seg->type;
            }
        }
    }
}

PUBLIC int query_memory_state(_IN void *start, _IN void *limit) {
    //TODO
    return MST_ERROR;
}

PUBLIC void *find_free_memory(_IN int size) {
    return NULL;
}