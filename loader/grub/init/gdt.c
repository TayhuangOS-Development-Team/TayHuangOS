/**
 * @file heap.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT - 实现
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <init/gdt.h>

descriptor_t GDT[64];
dptr_t GDTR;

static void init_empty_desc(void) PRIVATE {
    raw_desc_t empty = {};
    GDT[0] = build_descriptor(empty);
}

static void init_code_desc(void) PRIVATE {
    raw_desc_t code = {};

    code.base = 0;
    code.limit = 0xFFFFF;

    code.dpl = DPL0;
    code.type = DTYPE_XRCODE;

    code.system    = true; // data/code segment
    code.present   = false;
    code.avl       = false;
    code.lm        = false;
    code.db        = true;
    code.granulity = true;

    GDT[1] = build_descriptor(code);
}

static void init_data_desc(void) PRIVATE {
    raw_desc_t data = {};

    data.base = 0;
    data.limit = 0xFFFFF;

    data.dpl = DPL0;
    data.type = DTYPE_RWDATA;

    data.system    = true; // data/code segment
    data.present   = false;
    data.avl       = false;
    data.lm        = false;
    data.db        = true;
    data.granulity = true;

    GDT[2] = build_descriptor(data);
}

void init_gdt(void) INITIALIZER {
    init_empty_desc();
    init_code_desc();
    init_data_desc();

    GDTR.size = sizeof (GDT);
    GDTR.address = GDT;
    asm volatile ("lgdt %0" : : "m"(GDTR)); //加载GDT
}