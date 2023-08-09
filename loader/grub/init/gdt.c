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
Descriptor GDT[64];

/**
 * @brief GDTR
 * 
 */
DPTR GDTR;

/**
 * @brief 初始化空描述符
 * 
 */
static void InitEmptyDesc(void) {
    RawDesc empty = {};
    GDT[0] = BuildDesc(empty);
}

/**
 * @brief 初始化代码段描述符
 * 
 */
static void InitCodeDesc(void) {
    RawDesc code = {};

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

    GDT[CODE_IDX] = BuildDesc(code);
}

static void InitDataDesc(void) {
    RawDesc data = {};

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

    GDT[DATA_IDX] = BuildDesc(data);
}

static void InitKCodeDesc(void) {
    RawDesc kcode = {};

    kcode.base = 0;
    kcode.limit = 0xFFFFF; // flat-model

    kcode.dpl = DPL0;
    kcode.type = DTYPE_XRCODE; // execute-readable

    kcode.system    = true; // data/code segment
    kcode.present   = true;
    kcode.avl       = false;
    kcode.lm        = true; // 64bit
    kcode.db        = true;
    kcode.granulity = true;

    GDT[KERCODE_IDX] = BuildDesc(kcode);
}

static void InitKDataDesc(void) {
    RawDesc kdata = {};

    kdata.base = 0;
    kdata.limit = 0xFFFFF; // flat-model

    kdata.dpl = DPL0;
    kdata.type = DTYPE_RWDATA; //read-write

    kdata.system    = true; // data/code segment
    kdata.present   = true;
    kdata.avl       = false;
    kdata.lm        = true; // 64bit
    kdata.db        = true;
    kdata.granulity = true;

    GDT[KERDATA_IDX] = BuildDesc(kdata);
}

/**
 * @brief 初始化GDT
 * 
 */
void InitGDT(void) {
    InitEmptyDesc();
    InitCodeDesc();
    InitDataDesc();
    InitKCodeDesc();
    InitKDataDesc();

    GDTR.address = GDT;
    GDTR.size = sizeof(GDT) - 1;

    asm volatile ("lgdt %0" : : "m"(GDTR)); //加载GDT

    stds(DATA_IDX << 3);
    stfs(DATA_IDX << 3);
    stgs(DATA_IDX << 3);
}