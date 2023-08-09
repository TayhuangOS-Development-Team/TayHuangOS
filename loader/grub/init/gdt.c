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

    // 平坦模型
    code.base = 0;
    code.limit = 0xFFFFF;

    // 内核级可读可执行代码段
    code.dpl = DPL0;
    code.type = DTYPE_XRCODE;

    // 代码/数据段
    code.system    = true;

    // 存在
    code.present   = true;
    code.avl       = false;

    // 32位段
    code.lm        = false;
    code.db        = true;

    // 4KB粒度
    code.granulity = true;

    GDT[CODE_IDX] = BuildDesc(code);
}

static void InitDataDesc(void) {
    RawDesc data = {};

    // 平坦模型
    data.base = 0;
    data.limit = 0xFFFFF;

    // 内核级可读写数据段
    data.dpl = DPL0;
    data.type = DTYPE_RWDATA;

    // 代码/数据段
    data.system    = true;

    // 存在
    data.present   = true;
    data.avl       = false;

    // 32位段
    data.lm        = false;
    data.db        = true;

    // 4KB粒度
    data.granulity = true;

    GDT[DATA_IDX] = BuildDesc(data);
}

static void InitKCodeDesc(void) {
    RawDesc kcode = {};

    // 平坦模型
    kcode.base = 0;
    kcode.limit = 0xFFFFF;

    // 内核级可读可执行代码段
    kcode.dpl = DPL0;
    kcode.type = DTYPE_XRCODE;

    // 代码/数据段
    kcode.system    = true;

    // 存在
    kcode.present   = true;
    kcode.avl       = false;

    // 64位段
    kcode.lm        = true;
    kcode.db        = true;

    // 4KB粒度
    kcode.granulity = true;

    GDT[KERCODE_IDX] = BuildDesc(kcode);
}

static void InitKDataDesc(void) {
    RawDesc kdata = {};

    // 平坦模型
    kdata.base = 0;
    kdata.limit = 0xFFFFF; 

    // 内核级可读写数据段
    kdata.dpl = DPL0;
    kdata.type = DTYPE_RWDATA;

    // 代码/数据段
    kdata.system    = true;

    // 存在
    kdata.present   = true;
    kdata.avl       = false;

    // 64位段
    kdata.lm        = true;
    kdata.db        = true;

    // 4KB粒度
    kdata.granulity = true;

    GDT[KERDATA_IDX] = BuildDesc(kdata);
}

/**
 * @brief 初始化GDT
 * 
 */
void InitGDT(void) {
    // 初始化描述符
    InitEmptyDesc();
    InitCodeDesc();
    InitDataDesc();
    InitKCodeDesc();
    InitKDataDesc();

    // 初始化GDTR
    GDTR.address = GDT;
    GDTR.size = sizeof(GDT) - 1;

    asm volatile ("lgdt %0" : : "m"(GDTR)); //加载GDT

    // 设置段
    stds(DATA_IDX << 3);
    stes(DATA_IDX << 3);
    stfs(DATA_IDX << 3);
    stgs(DATA_IDX << 3);
    stss(DATA_IDX << 3);
}