// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/pm/a20.h
 *
 * A20函数
 */



#pragma once

#include "../header.h"

PUBLIC bool test_a20(int loop_time); //测试a20是否开启
PUBLIC bool test_a20_fast(void); //快速测试
PUBLIC bool test_a20_normal(void); //正常测试
PUBLIC bool test_a20_slow(void); //慢测试
PUBLIC bool test_a20_long(void); //久测试
PUBLIC bool empty_8042(void); //清空8042
PUBLIC void enable_a20_by_bios(void); //用bios中断打开A20
PUBLIC void enable_a20_by_8042(void); //靠8042打开A20
PUBLIC void enable_a20_by_portA(void); //靠port A打开A20
PUBLIC bool enable_a20(void);