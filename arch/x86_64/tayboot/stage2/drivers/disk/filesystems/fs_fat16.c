// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/disk/filesystems/fs_fat16.h
 *
 * Fat 16 file system functions are implemented here
 */



#include "fs_fat16.h"
#include "../../../buffer.h"
#include "../../../heap.h"
#include "../../../printf.h"
#include "../disk_driver.h"
#include <string.h>

PRIVATE const char pattern[] = "FAT16   ";

PUBLIC bool chk_is_fat16(addr_t first_sector) {
    for (int i = 0 ; i < 8 ; i ++) {
        if (pattern[i] != get_buffer_byte(first_sector + 0x36 + i)) {
            return false;
        }
    }
    return true;
}

PUBLIC void* create_fat16_file_system(addr_t first_sector) {
    fs_fat16_t *fat16_fs = malloc(sizeof(fs_fat16_t));
    fat16_fs->magic = 0xD949FA99;
    cp_from_buffer(first_sector + 0x3, &fat16_fs->oem_name, 8);
    fat16_fs->oem_name[8] = 0;
    fat16_fs->bytes_per_sector = get_buffer_word(first_sector + 0xB);
    fat16_fs->sectors_per_clus = get_buffer_byte(first_sector + 0xD);
    fat16_fs->reversed_sectors = get_buffer_word(first_sector + 0xE);
    fat16_fs->num_fats = get_buffer_byte(first_sector + 0x10);
    fat16_fs->root_entries = get_buffer_word(first_sector + 0x11);
    if (get_buffer_word(first_sector + 0x13)) {
        fat16_fs->total_sectors = get_buffer_word(first_sector + 0x13);
    }
    else {
        fat16_fs->total_sectors = get_buffer_dword(first_sector + 0x20);
    }
    fat16_fs->media = get_buffer_byte(first_sector + 0x15);
    fat16_fs->fat_sectors = get_buffer_word(first_sector + 0x16);
    fat16_fs->sectors_per_track = get_buffer_word(first_sector + 0x18);
    fat16_fs->heads = get_buffer_word(first_sector + 0x1A);
    fat16_fs->hidden_sectors = get_buffer_dword(first_sector + 0x1C);
    fat16_fs->drive_no = get_buffer_byte(first_sector + 0x24);
    fat16_fs->boot_sig = get_buffer_byte(first_sector + 0x26);
    fat16_fs->volumn_id = get_buffer_byte(first_sector + 0x27);
    cp_from_buffer(first_sector + 0x2B, &fat16_fs->volumn_label, 11);
    fat16_fs->volumn_label[11] = 0;
    cp_from_buffer(first_sector + 0x36, &fat16_fs->file_system, 8);
    fat16_fs->file_system[8] = 0;
    fat16_fs->fat1_start = fat16_fs->reversed_sectors;
    fat16_fs->root_directory_start = fat16_fs->num_fats * fat16_fs->fat_sectors + fat16_fs->fat1_start;
    fat16_fs->entries_per_sector = fat16_fs->bytes_per_sector / FAT16_ROOT_ENTRY_SIZE;
    fat16_fs->root_directory_sectors = fat16_fs->root_entries / fat16_fs->entries_per_sector;
    fat16_fs->data_start = fat16_fs->root_directory_start + fat16_fs->root_directory_sectors - 2;
    return fat16_fs;
}

PUBLIC void print_fat16_file_system(const pfs_fat16 fat16_fs) {
    printf ("OEM name: %s\n", fat16_fs->oem_name);
    printf ("Bytes per sector: %d\n", fat16_fs->bytes_per_sector);
    printf ("Sectors per clus: %d\n", fat16_fs->sectors_per_clus);
    printf ("Reversed sectors: %d\n", fat16_fs->reversed_sectors);
    printf ("Num FATs: %d\n", fat16_fs->num_fats);
    printf ("Root entries: %d\n", fat16_fs->root_entries);
    printf ("Total sectors: %#8X\n", fat16_fs->total_sectors);
    printf ("Media: %#2X\n", fat16_fs->media);
    printf ("Fat sectors: %d\n", fat16_fs->fat_sectors);
    printf ("Sectors per track: %d\n", fat16_fs->sectors_per_track);
    printf ("Heads: %d\n", fat16_fs->heads);
    printf ("Hidden sectors: %d\n", fat16_fs->hidden_sectors);
    printf ("Drive No: %#2X\n", fat16_fs->drive_no);
    printf ("Boot sig: %#2X\n", fat16_fs->boot_sig);
    printf ("Volumn Id: %d\n", fat16_fs->volumn_id);
    printf ("Volumn label: %s\n", fat16_fs->volumn_label);
    printf ("File system: %s\n", fat16_fs->file_system);
    printf ("FAT1 start: %#4X\n", fat16_fs->fat1_start);
    printf ("Root directory start: %#4X\n", fat16_fs->root_directory_start);
    printf ("Root directory sectors: %#4X\n", fat16_fs->root_directory_sectors);
    printf ("Data start: %#4X\n", fat16_fs->data_start);
    printf ("Entries per sector: %d\n", fat16_fs->entries_per_sector);
}

PUBLIC word get_fat16_entry(word no, pdriver disk) {
    addr_t buffer = alloc_buffer(512, false);
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    dword sector = (fs->fat1_start) + (no >> 8);
    read_sector(disk, buffer, 0, sector);
    word result = get_buffer_word(buffer + (no & 0xFF) * 2);
    free_buffer(buffer);
    return result;
}

PUBLIC word get_fat16_file_no(const char *filename, pdriver disk) {
    if (strlen(filename) != 11)
        return 0xFFFF;
    char name[12] = "";
    name[11] = 0;
    short entry_no = 0;
    short sector_entry = 0xFFFF;
    short offset_of_entry = 0;
    addr_t buffer = alloc_buffer(512, false);
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    while (entry_no < fs->root_entries) {
        if ((entry_no / fs->entries_per_sector) != sector_entry) {
            sector_entry = entry_no / fs->entries_per_sector;
            read_sector(disk, buffer, 0, sector_entry + fs->root_directory_start);
        }
        offset_of_entry = (entry_no % fs->entries_per_sector) * FAT16_ROOT_ENTRY_SIZE;
        cp_from_buffer(buffer + offset_of_entry, name, 11);
        if (! strcmp(filename, name)) {
            return entry_no;
        }
        entry_no ++;
    }
    free_buffer (buffer);
    return 0xFFFF;
}

PUBLIC void get_fat16_file_info_by_name(const char *filename, pdriver disk, pfat16_file file) {
    word entry_no = get_fat16_file_no(filename, disk);
    if (entry_no == 0xFFFF) {
        file->length = -1;
        return;
    }
    get_fat16_file_info(entry_no, disk, file);
}

PUBLIC void get_fat16_file_info(word entry_no, pdriver disk, pfat16_file file) {
    addr_t buffer = alloc_buffer(512, false);
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    word sector_entry = entry_no / fs->entries_per_sector;
    read_sector(disk, buffer, 0, sector_entry + fs->root_directory_start);
    word offset_of_entry = (entry_no % fs->entries_per_sector) * FAT16_ROOT_ENTRY_SIZE;
    cp_from_buffer(buffer + offset_of_entry, &file->name, 11);
    byte attribute = get_buffer_byte(buffer + offset_of_entry + 0xB);
    file->attribute.read_only = attribute & 1;
    file->attribute.hidden = (attribute >> 1) & 1;
    file->attribute.system = (attribute >> 2) & 1;
    file->attribute.vol = (attribute >> 3) & 1;
    file->attribute.directory = (attribute >> 4) & 1;
    file->attribute.archive = (attribute >> 5) & 1;
    file->latest_modify_time = get_buffer_word(buffer + offset_of_entry + 0x16);
    file->latest_modify_date = get_buffer_word(buffer + offset_of_entry + 0x18);
    file->first_clus = get_buffer_word(buffer + offset_of_entry + 0x1A);
    file->length = get_buffer_dword(buffer + offset_of_entry + 0x1C);
    free_buffer(buffer);
}

PUBLIC void set_fat16_entry(word no, word nxt, pdriver disk) {
    addr_t buffer = alloc_buffer(512, false);
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    dword sector = (fs->fat1_start) + (no >> 8);
    read_sector(disk, buffer, 0, sector);
    set_buffer_word(buffer + (no & 0xFF) * 2, nxt);
    save_sector(disk, buffer, 0, sector);
    free_buffer(buffer);
}

PUBLIC word append_new_sector(word last, pdriver disk) {
    word free = find_free_entry(disk);
    set_fat16_entry(last, free, disk);
    set_fat16_entry(free, 0xFF, disk);
    return free;
}

PUBLIC void set_fat16_file_info(word entry_no, pdriver disk, pfat16_file file) {
    addr_t buffer = alloc_buffer(512, false);
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    word sector_entry = entry_no / fs->entries_per_sector;
    word offset_of_entry = (entry_no % fs->entries_per_sector) * FAT16_ROOT_ENTRY_SIZE;
    read_sector(disk, buffer, 0, fs->root_directory_start + sector_entry);
    cp_to_buffer(&file->name, buffer + offset_of_entry, 11);
    set_buffer_byte(buffer + offset_of_entry + 0xB,
        (file->attribute.read_only) |
        (file->attribute.hidden << 1) | 
        (file->attribute.system << 2) | 
        (file->attribute.vol << 3) | 
        (file->attribute.directory << 4) | 
        (file->attribute.archive << 5));
    set_buffer_word(buffer + offset_of_entry + 0x16, file->latest_modify_time);
    set_buffer_word(buffer + offset_of_entry + 0x18, file->latest_modify_date);
    set_buffer_word(buffer + offset_of_entry + 0x1A, file->first_clus);
    set_buffer_dword(buffer + offset_of_entry + 0x1C, file->length);
    save_sector(disk, buffer, 0, fs->root_directory_start + sector_entry);
    free_buffer(buffer);
}

PUBLIC void set_fat16_file_info_by_name(const char *filename, pdriver disk, const pfat16_file file) {
    word entry_no = get_fat16_file_no(filename, disk);
    if (entry_no == 0xFFFF) {
        file->length = 0xFFFF;
        return;
    }
    set_fat16_file_info(entry_no, disk, file);
}

PUBLIC word find_free_file_entry(pdriver disk) {
    fat16_file_t file;
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    for (int i = 0 ; i < fs->root_entries ; i ++) {
        get_fat16_file_info(i, disk, &file);
        if (file.name[0] == '\0') return i;
    }
    return 0xFFFF;
}

PUBLIC bool insert_fat16_file_info(pdriver disk, pfat16_file file) {
    word entry_no = find_free_file_entry(disk);
    if (entry_no == 0xFFFF) {
        return false;
    }
    set_fat16_file_info(entry_no, disk, file);
    return true;
}

PUBLIC word find_free_entry(pdriver disk) {
    pfs_fat16 fs;
    disk->pc_handle(disk, DK_CMD_GET_FILESYSTEM, &fs);
    for (int i = 3 ; i < fs->fat_sectors * (512 / 2) ; i ++) {
        if (get_fat16_entry(i, disk) == 0) {
            return i;
        }
    }
    return 0xFFFF;
}