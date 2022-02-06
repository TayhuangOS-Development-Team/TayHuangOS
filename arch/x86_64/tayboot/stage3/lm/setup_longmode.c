/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/lm/setup_longmode.c
 *
 * 设置长模式
 *
 */



#include "setup_longmode.h"
#include <tayboot/control_registers.h>
#include "../printf.h"

PUBLIC void setup_longmode(void *pml4) {
    cr0_t cr0;
    *((int*)&cr0) = get_cr0();
    cr3_t cr3;
    *((int*)&cr3) = get_cr3();
    cr4_t cr4;
    *((int*)&cr4) = get_cr4();
    efer_t efer;
    *((int*)&efer) = get_efer();
    *((int*)&cr3) |= (dword)pml4;
    set_cr3(*((int*)&cr3));
    cr4.pae = true; //物理地址拓展
    set_cr4(*((int*)&cr4));
    efer.lme = true; //Long Mode Enable
    set_efer(*((int*)&efer));
    cr0.pg = true; //启用分页 LMA自动为1
    set_cr0(*((int*)&cr0)); 
}