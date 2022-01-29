// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/include/tayhuang/ports.h
 *
 * Basic int vectors
 */



#pragma once

#define CALC_IRQ(x) (0x20 + x)
#define INT_VECTOR_IRQ0 CALC_IRQ(0)
#define INT_VECTOR_IRQ1 CALC_IRQ(1)
#define INT_VECTOR_IRQ2 CALC_IRQ(2)
#define INT_VECTOR_IRQ3 CALC_IRQ(3)
#define INT_VECTOR_IRQ4 CALC_IRQ(4)
#define INT_VECTOR_IRQ5 CALC_IRQ(5)
#define INT_VECTOR_IRQ6 CALC_IRQ(6)
#define INT_VECTOR_IRQ7 CALC_IRQ(7)
#define INT_VECTOR_IRQ8 CALC_IRQ(8)
#define INT_VECTOR_IRQ9 CALC_IRQ(9)
#define INT_VECTOR_IRQ10 CALC_IRQ(10)