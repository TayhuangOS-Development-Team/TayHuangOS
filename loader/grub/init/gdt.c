/**
 * @file gdt.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT - 实现
 * @version alpha-1.0.0
 * @date 2023-3-21
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
    code.limit = 0xFFFFF; // flat-model

    code.dpl = DPL0;
    code.type = DTYPE_XRCODE; // execute-readable

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
    data.limit = 0xFFFFF; // flat-model

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

static void init_kercode_desc(void) PRIVATE {
    raw_desc_t kercode = {};

    kercode.base = 0;
    kercode.limit = 0xFFFFF; // flat-model

    kercode.dpl = DPL0;
    kercode.type = DTYPE_XRCODE; // execute-readable

    kercode.system    = true; // data/code segment
    kercode.present   = false;
    kercode.avl       = false;
    kercode.lm        = true; // 64bit
    kercode.db        = true;
    kercode.granulity = true;

    GDT[8] = build_descriptor(kercode);
}

static void init_kerdata_desc(void) PRIVATE {
    raw_desc_t kerdata = {};

    kerdata.base = 0;
    kerdata.limit = 0xFFFFF; // flat-model

    kerdata.dpl = DPL0;
    kerdata.type = DTYPE_RWDATA; //read-write

    kerdata.system    = true; // data/code segment
    kerdata.present   = false;
    kerdata.avl       = false;
    kerdata.lm        = true; // 64bit
    kerdata.db        = true;
    kerdata.granulity = true;

    GDT[9] = build_descriptor(kerdata);
}

void init_gdt(void) INITIALIZER {
    init_empty_desc();
    init_code_desc();
    init_data_desc();
    init_kercode_desc();
    init_kerdata_desc();

    GDTR.size = sizeof (GDT);
    GDTR.address = GDT;
    asm volatile ("lgdt %0" : : "m"(GDTR)); //加载GDT
}