/**
 * @file gdt.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GDT
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/descs.h>

#define GDT_SIZE (16)

/**
 * @brief GDT
 * 
 */
EXTERN PUBLIC struct desc_struct GDT[GDT_SIZE];
/**
 * @brief GDTR GDT指针
 * 
 */
EXTERN PUBLIC struct gdt_ptr gdtr;

/**
 * @brief 初始化GDT
 * 
 */
PUBLIC void init_gdt(void);