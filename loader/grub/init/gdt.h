/**
 * @file gdt.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT
 * @version alpha-1.0.0
 * @date 2023-3-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/desc.h>

/** GDT */
extern desc_t GDT[64];

/** GDTR */
extern dptr_t GDTR;

/**
 * @brief 初始化GDT
 * 
 */
void init_gdt(void);