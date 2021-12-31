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
 * Fat 16 file system functions are implemented here
 */



#include "fs_fat16.h"
#include "../../../heap.h"
#include "../../../printf.h"

PRIVATE const char pattern[] = "FAT16   ";

PUBLIC bool chk_is_fat16(addr_t first_sector) {
    for (int i = 0 ; i < 8 ; i ++) {
        if (pattern[i] != get_heap_byte(first_sector + 0x36 + i)) {
            return false;
        }
    }
    return true;
}

PUBLIC addr_t create_fat16_file_system(addr_t first_sector) {
    fs_fat16_t _fat16_fs;
    _fat16_fs.magic = 0xD949FA99;
    cp_from_heap(first_sector + 0x3, &_fat16_fs.oem_name, 8);
    _fat16_fs.oem_name[8] = 0;
    _fat16_fs.bytes_per_sector = get_heap_word(first_sector + 0xB);
    _fat16_fs.sectors_per_clus = get_heap_byte(first_sector + 0xD);
    _fat16_fs.reversed_sectors = get_heap_word(first_sector + 0xE);
    _fat16_fs.num_fats = get_heap_byte(first_sector + 0x10);
    _fat16_fs.root_entries = get_heap_word(first_sector + 0x11);
    if (get_heap_word(first_sector + 0x13)) {
        _fat16_fs.total_sectors = get_heap_word(first_sector + 0x13);
    }
    else {
        _fat16_fs.total_sectors = get_heap_dword(first_sector + 0x20);
    }
    _fat16_fs.media = get_heap_byte(first_sector + 0x15);
    _fat16_fs.fat_sectors = get_heap_word(first_sector + 0x16);
    _fat16_fs.sectors_per_track = get_heap_word(first_sector + 0x18);
    _fat16_fs.heads = get_heap_word(first_sector + 0x1A);
    _fat16_fs.hidden_sectors = get_heap_dword(first_sector + 0x1C);
    _fat16_fs.drive_no = get_heap_byte(first_sector + 0x24);
    _fat16_fs.boot_sig = get_heap_byte(first_sector + 0x26);
    _fat16_fs.volumn_id = get_heap_byte(first_sector + 0x27);
    cp_from_heap(first_sector + 0x2B, &_fat16_fs.volumn_label, 11);
    _fat16_fs.volumn_label[11] = 0;
    cp_from_heap(first_sector + 0x36, &_fat16_fs.file_system, 8);
    _fat16_fs.file_system[8] = 0;
    addr_t fat16_fs = alloc(sizeof(fs_fat16_t), false);
    cp_to_heap(&_fat16_fs, fat16_fs, sizeof(fs_fat16_t));
    return fat16_fs;
}

PUBLIC void print_fat16_file_system(addr_t fat16_fs) {
    printf ("OEM name: %s\n", ((pfs_fat16)LDADDR(fat16_fs))->oem_name);
    printf ("Bytes per sector: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->bytes_per_sector);
    printf ("Sectors per clus: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->sectors_per_clus);
    printf ("Reversed sectors: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->reversed_sectors);
    printf ("Num FATs: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->num_fats);
    printf ("Root entries: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->root_entries);
    printf ("Total sectors: %#8X\n", ((pfs_fat16)LDADDR(fat16_fs))->total_sectors);
    printf ("Media: %#2X\n", ((pfs_fat16)LDADDR(fat16_fs))->media);
    printf ("Fat sectors: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->fat_sectors);
    printf ("Sectors per track: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->sectors_per_track);
    printf ("Heads: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->heads);
    printf ("Hidden sectors: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->hidden_sectors);
    printf ("Drive No: %#2X\n", ((pfs_fat16)LDADDR(fat16_fs))->drive_no);
    printf ("Boot sig: %#2X\n", ((pfs_fat16)LDADDR(fat16_fs))->boot_sig);
    printf ("Volumn Id: %d\n", ((pfs_fat16)LDADDR(fat16_fs))->volumn_id);
    printf ("Volumn Label: %s\n", ((pfs_fat16)LDADDR(fat16_fs))->volumn_label);
    printf ("File System: %s\n", ((pfs_fat16)LDADDR(fat16_fs))->file_system);
}