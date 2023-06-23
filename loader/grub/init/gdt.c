/**
 * @file gdt.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT
 * @version alpha-1.0.0
 * @date 2023-3-21
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <init/gdt.h>
#include <tay/io.h>

#define EMPTY_IDX (0)
#define CODE_IDX (2)
#define DATA_IDX (3)
#define KERCODE_IDX (8)
#define KERDATA_IDX (9)

/**
 * @brief GDT
 * 
 */
desc_t GDT[64];

/**
 * @brief GDTR
 * 
 */
dptr_t GDTR;

/**
 * @brief 初始化空描述符
 * 
 */
static void init_empty_desc(void) {
    raw_desc_t empty = {};
    GDT[0] = build_descriptor(empty);
}

/**
 * @brief 初始化代码段描述符
 * 
 */
static void init_code_desc(void) {
    raw_desc_t code = {};

    code.base = 0;
    code.limit = 0xFFFFF; // flat-model

    code.dpl = DPL0;
    code.type = DTYPE_XRCODE; // execute-readable

    code.system    = true; // data/code segment
    code.present   = true;
    code.avl       = false;
    code.lm        = false;
    code.db        = true;
    code.granulity = true;

    GDT[CODE_IDX] = build_descriptor(code);
}

static void init_data_desc(void) {
    raw_desc_t data = {};

    data.base = 0;
    data.limit = 0xFFFFF; // flat-model

    data.dpl = DPL0;
    data.type = DTYPE_RWDATA;

    data.system    = true; // data/code segment
    data.present   = true;
    data.avl       = false;
    data.lm        = false;
    data.db        = true;
    data.granulity = true;

    GDT[DATA_IDX] = build_descriptor(data);
}

static void init_kercode_desc(void) {
    raw_desc_t kercode = {};

    kercode.base = 0;
    kercode.limit = 0xFFFFF; // flat-model

    kercode.dpl = DPL0;
    kercode.type = DTYPE_XRCODE; // execute-readable

    kercode.system    = true; // data/code segment
    kercode.present   = true;
    kercode.avl       = false;
    kercode.lm        = true; // 64bit
    kercode.db        = true;
    kercode.granulity = true;

    GDT[KERCODE_IDX] = build_descriptor(kercode);
}

static void init_kerdata_desc(void) {
    raw_desc_t kerdata = {};

    kerdata.base = 0;
    kerdata.limit = 0xFFFFF; // flat-model

    kerdata.dpl = DPL0;
    kerdata.type = DTYPE_RWDATA; //read-write

    kerdata.system    = true; // data/code segment
    kerdata.present   = true;
    kerdata.avl       = false;
    kerdata.lm        = true; // 64bit
    kerdata.db        = true;
    kerdata.granulity = true;

    GDT[KERDATA_IDX] = build_descriptor(kerdata);
}

/**
 * @brief 初始化GDT
 * 
 */
void init_gdt(void) {
    init_empty_desc();
    init_code_desc();
    init_data_desc();
    init_kercode_desc();
    init_kerdata_desc();

    GDTR.address = GDT;
    GDTR.size = sizeof(GDT) - 1;

    asm volatile ("lgdt %0" : : "m"(GDTR)); //加载GDT

    stds(DATA_IDX << 3);
    stfs(DATA_IDX << 3);
    stgs(DATA_IDX << 3);
}