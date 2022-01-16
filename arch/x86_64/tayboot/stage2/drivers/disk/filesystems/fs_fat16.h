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
    dword fat1_start;
    dword root_directory_start;
    dword root_directory_sectors;
    dword data_start;
    word entries_per_sector;
} fs_fat16_t, *pfs_fat16;

#define FAT16_ROOT_ENTRY_SIZE (0x20)

typedef struct {
    char name[12];
    struct {
        bool read_only : 1;
        bool hidden : 1;
        bool system : 1;
        bool vol : 1;
        bool directory : 1;
        bool archive : 1;
    } attribute;
    char reversed[10];
    word latest_modify_time;
    word latest_modify_date;
    word first_clus;
    dword length;
} fat16_file_t, *pfat16_file;

PUBLIC bool chk_is_fat16(addr_t first_sector);
PUBLIC addr_t create_fat16_file_system(addr_t first_sector);
PUBLIC void print_fat16_file_system(addr_t fat16_fs);
PUBLIC word get_fat16_entry(word no, pdriver disk);
PUBLIC void get_fat16_file_info(const char *filename, pdriver disk, pfat16_file file);