// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/disk/disk_driver.h
 *
 * Disk driver is declared here
 */



#pragma once

#include "../driver.h"

#define DK_CMD_READ_SECTOR CMD_TY(0)

PUBLIC void create_disk_driver(pdriver driver);