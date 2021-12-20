// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/intcall.h
 *
 * Int call function is declared here
 */



#pragma once

#include "header.h"

typedef struct {
    reg32_t eax;
    reg32_t ebx;
    reg32_t ecx;
    reg32_t edx;
    reg32_t edi;
    reg32_t esi;
    sreg_t es;
    sreg_t fs;
    sreg_t gs;
}reg_collect_t, *preg_collect;

typedef struct {
    preg_collect in_regs;
    preg_collect out_regs;
    int int_no;
}intargs_t, *pintargs;

PUBLIC void intcall(pintargs args);