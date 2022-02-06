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
 * arch/x86_64/tayboot/stage2/drivers/disk/filesystem.h
 *
 * 文件系统
 */



#include "filesystem.h"
#include "../../buffer.h"
#include "disk_driver.h"
#include "filesystems/fs_fat16.h"

PUBLIC void *recognize_file_system(pdriver disk_driver) {
    addr_t sector_addr = alloc_buffer(512, false);

    APACK(dk, read_sector) pack;
    pack.dst = sector_addr;
    pack.src_low = pack.src_high = 0;
    disk_driver->pc_handle(disk_driver, DK_CMD_READ_SECTOR, &pack);

    if (chk_is_fat16(sector_addr)) {
        void *fs_fat16 = create_fat16_file_system(sector_addr);
        free_buffer (sector_addr);
        return fs_fat16;
    }
    return NULL;
}