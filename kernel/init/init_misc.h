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
 * init_misc.h
 *
 * 杂项初始化
 *
 */



#pragma once

#include <tayhuang/control_registers.h>

static inline void init_sse(void) { //SSE
    cr0_t cr0 = get_cr0();
    cr0.em = 0;
    cr0.mp = 1;
    set_cr0(cr0);

    cr4_t cr4 = get_cr4();
    cr4.osfxsr = 1;
    cr4.osxmmexcpt = 1;
    set_cr4(cr4);
}