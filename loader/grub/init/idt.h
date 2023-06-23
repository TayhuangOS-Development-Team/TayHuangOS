/**
 * @file idt.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief IDT
 * @version alpha-1.0.0
 * @date 2023-05-01
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/desc.h>

/** IDT */
extern gate_desc_t IDT[256];

/** IDTR */
extern dptr_t IDTR;

/**
 * @brief 初始化PIC
 * 
 */
void init_pic(void);

/**
 * @brief 初始化IDT
 * 
 */
void init_idt(void);