// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/driver.c
 *
 * Driver functions are implemented here
 */



#include "driver.h"

PRIVATE id_t cur_id = 0;

PUBLIC id_t alloc_id() {
    return cur_id ++;
}