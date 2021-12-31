// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/disk/filesystems/fs_fat16.h
 *
 * Fat 16 file system functions are declared here
 */



#pragma once

#include "../filesystem.h"

typedef struct {
    dword magic; //always 0xD949FA99 (The first eight bits of MD5 of "File Allocation Table 16")
    char oem_name[8 + 1];
    word bytes_per_sector;
    byte sectors_per_clus;
    word reversed_sectors;
    byte num_fats;
    word root_entries;
    dword total_sectors;
    byte media;
    word fat_sectors;
    word sectors_per_track;
    word heads;
    dword hidden_sectors;
    byte drive_no;
    byte boot_sig;
    dword volumn_id;
    char volumn_label[11 + 1];
    char file_system[8 + 1];
} fs_fat16_t, *pfs_fat16;

PUBLIC bool chk_is_fat16(addr_t first_sector);
PUBLIC addr_t create_fat16_file_system(addr_t first_sector);
PUBLIC void print_fat16_file_system(addr_t fat16_fs);