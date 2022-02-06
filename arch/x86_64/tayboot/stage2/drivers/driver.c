/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * driver.c
 *
 * 驱动相关函数
 *
 */



#include "driver.h"

PRIVATE id_t cur_id = 0;

PUBLIC id_t alloc_id() {
    return cur_id ++;
}