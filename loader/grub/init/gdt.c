/**
 * @file gdt.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <init/gdt.h>
#include <tayhuang/io.h>
#include <tayhuang/control_registers.h>

struct desc_struct GDT[GDT_SIZE];

struct gdt_ptr gdtr;

PUBLIC void init_gdt(void) {
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0xFFFFF); //空
    GDT[1] = (struct desc_struct)GDT_ENTRY(0xC09A, 0, 0xFFFFF); //代码段
    GDT[2] = (struct desc_struct)GDT_ENTRY(0xC093, 0, 0xFFFFF); //数据段
    GDT[3] = (struct desc_struct)GDT_ENTRY(0x0089, 4096, 103); //TSS
    GDT[7] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF); //代码段64bit
    GDT[8] = (struct desc_struct)GDT_ENTRY(0xC093, 0, 0xFFFFF); //数据段64bit
    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT) - 1;
    asmv ("lgdtl %0" : : "m"(gdtr)); //加载GDT
    stds(2 << 3);

    flush_cs(1 << 3);
}
