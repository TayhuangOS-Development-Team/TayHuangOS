// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/disk.c
 *
 * Disk functions are implemented here
 */



#include "disk.h"
#include "memory.h"
#include "string.h"
#include "print.h"

struct FsInfo {
    char oem_name[8];
    word bytes_per_sector;
    byte sectors_per_clus;
    word rsvd_sectors;
    byte fats_num;
    word root_entries;
    word sectors_small;
    byte media;
    word fat_sectors;
    word sectors_per_track;
    word heads;
    dword hidden_sectors;
    dword sectors_large;
    byte drive;
    byte bootsig;
    dword volumn_id;
    char volumn_label[11];
    char filesystem[8];
} fs_info;

struct ExtFsInfo {
    word root_dir_start;
    word entries_per_sector;
    word root_sector_num;
    word data_start;
} ext_fs_info;

void _readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset);

void readsector(dword LDA_LOW, dword LDA_HIGH, word segment, word offset) {
    _readsector(LDA_LOW, LDA_HIGH, segment, offset);
    ed_callasm();
}

void initfsinfo(void) {
    readsector(0, 0, BASE_OF_DATA, 0);
    stfs(BASE_OF_DATA);
    rdfsn((byte *) fs_info.oem_name, 8, 0x03);
    fs_info.bytes_per_sector = rdfs16(0x0B);
    fs_info.sectors_per_clus = rdfs8(0x0D);
    fs_info.rsvd_sectors = rdfs16(0x0E);
    fs_info.fats_num = rdfs8(0x10);
    fs_info.root_entries = rdfs16(0x11);
    fs_info.sectors_small = rdfs16(0x13);
    fs_info.media = rdfs8(0x15);
    fs_info.fat_sectors = rdfs16(0x16);
    fs_info.sectors_per_track = rdfs16(0x18);
    fs_info.heads = rdfs16(0x1A);
    fs_info.hidden_sectors = rdfs32(0x1C);
    fs_info.sectors_large = rdfs32(0x20);
    fs_info.drive = rdfs8(0x24);
    fs_info.bootsig = rdfs8(0x26);
    fs_info.volumn_id = rdfs32(0x27);
    rdfsn((byte *) fs_info.volumn_label, 11, 0x2B);
    rdfsn((byte *) fs_info.filesystem, 8, 0x36);
    ext_fs_info.entries_per_sector = 512 / 32;
    ext_fs_info.root_dir_start = fs_info.fats_num * fs_info.fat_sectors + 1;
    ext_fs_info.root_sector_num = fs_info.root_entries / ext_fs_info.entries_per_sector;
    ext_fs_info.data_start = ext_fs_info.root_dir_start + ext_fs_info.root_sector_num - 2;
}

void printfsinfo(void) {
    printstr("oem_name: ");
    for (int i = 0 ; i < 8 ; i ++) {
        printchar(*(fs_info.oem_name + i));
    }
    printreturn();

    printstr("bytes per sector: ");
    printshort(fs_info.bytes_per_sector, true);
    printreturn();

    printstr("sectors per clus: ");
    printbyte(fs_info.sectors_per_clus, true);
    printreturn();

    printstr("rsvd sectors: ");
    printshort(fs_info.rsvd_sectors, true);
    printreturn();

    printstr("num of fats: ");
    printbyte(fs_info.fats_num, true);
    printreturn();

    printstr("root entries: ");
    printshort(fs_info.root_entries, true);
    printreturn();

    printstr("sectors: ");
    if (fs_info.sectors_small == 0) {
        printint(fs_info.sectors_large, true);
    }
    else {
        printshort(fs_info.sectors_small, false);
    }
    printreturn();

    printstr("root entries: ");
    printshort(fs_info.root_entries, true);
    printreturn();

    printstr("media: ");
    printbytehex(fs_info.media, true);
    printreturn();

    printstr("fat sectors: ");
    printshort(fs_info.fat_sectors, true);
    printreturn();

    printstr("sectors per track: ");
    printshort(fs_info.sectors_per_track, true);
    printreturn();

    printstr("heads: ");
    printshort(fs_info.heads, true);
    printreturn();

    printstr("hidden sectors:");
    printint(fs_info.hidden_sectors, true);
    printreturn();

    printstr("drive:");
    printbytehex(fs_info.drive, true);
    printreturn();

    printstr("bootsig:");
    printbytehex(fs_info.bootsig, true);
    printreturn();

    printstr("volumn id:");
    printinthex(fs_info.volumn_id, true);
    printreturn();

    printstr("volumn_label: ");
    for (int i = 0 ; i < 11 ; i ++) {
        printchar(*(fs_info.volumn_label + i));
    }
    printreturn();

    printstr("file_system: ");
    for (int i = 0 ; i < 8 ; i ++) {
        printchar(*(fs_info.filesystem + i));
    }
    printreturn();

    printstr("entries per sector: ");
    printshort(ext_fs_info.entries_per_sector, true);
    printreturn();

    printstr("root dir start: ");
    printshort(ext_fs_info.root_dir_start, true);
    printreturn();

    printstr("root sector num: ");
    printshort(ext_fs_info.root_sector_num, true);
    printreturn();

    printstr("root data start: ");
    printshort(ext_fs_info.data_start, true);
    printreturn();
}

short getfatentry(short clus) {
    static short sector_of_entry = -1;
    short offset_of_entry = (clus & 0xFF) * 2;
    if (sector_of_entry != clus >> 8) {
        sector_of_entry = clus >> 8;
        readsector(fs_info.rsvd_sectors + sector_of_entry, 0, BASE_OF_DATA, 0);
    }
    stfs(BASE_OF_DATA);
    return rdfs16(offset_of_entry);
}

short findfile(const char *filename) {
    if (strlen(filename) != 11)
        return 0xFFFF;
    char name[12] = "";
    name[11] = '\0';
    short entry_no = 0;
    short sector_entry = -1;
    short offset_of_entry;
    stfs(BASE_OF_DATA);
    while (entry_no < fs_info.root_entries) {
        if (entry_no / ext_fs_info.entries_per_sector != sector_entry) {
            sector_entry = entry_no / ext_fs_info.entries_per_sector;
            readsector(sector_entry + ext_fs_info.root_dir_start, 0, BASE_OF_DATA, 0);
        }
        offset_of_entry = (entry_no % ext_fs_info.entries_per_sector) * 32;
        rdfsn((byte*)name, 11, offset_of_entry);
        if (! strcmp(filename, name)) {
            return rdfs16(offset_of_entry + 0x1A);
        }
        entry_no ++;
    }
    return 0xFFFF;
}

void loadfile(const char *filename, word segment, word offset) {
    word clus = findfile(filename);
    while (clus < (word)0xFFF8) {
        readsector(ext_fs_info.data_start + clus, 0, segment, offset);
        offset += 512;
        if (clus != 0) {
            printshorthex(clus, true);
            printreturn();
        }
        clus = getfatentry(clus);
    }
}