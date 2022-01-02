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
#define DK_CMD_INIT CMD_TY(1)
#define DK_CMD_GET_FILESYSTEM CMD_TY(2)
#define DK_CMD_LOAD_FILE CMD_TY(3)

typedef struct {
    addr_t dst;
    dword src_high;
    dword src_low;
} DAPACK(dk, read_sector);

typedef struct {
    sreg_t segment;
    addr_t offset;
    const char* name;
} DAPACK(dk, load_file);

PUBLIC void create_disk_driver(pdriver driver);
PUBLIC void read_sector(pdriver driver, addr_t buffer, dword lba_high, dword lba_low);