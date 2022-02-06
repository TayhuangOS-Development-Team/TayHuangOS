/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/fs/fat32.c
 *
 * FAT32文件系统
 *
 */



#include "fat32.h"
#include "../memory.h"
#include "../disk.h"
#include "../printf.h"
#include <ctype.h>
#include <string.h>

typedef struct {
    // 
    int selector; //Selector of Context
    dword magic; //0x93186A8E (MD5 of "File Allocation Table 32"的前32位)
    // 
    char oem_name[8 + 1]; //oem名
    word bytes_per_sector; //每扇区字节数
    byte sectors_per_clus; //每簇扇区数
    word reversed_sectors; //保留扇区
    byte num_fats; //fat数
    dword total_sectors; //总扇区数
    byte media; //介质
    dword fat_sectors; //fat扇区数
    word sectors_per_track; //每磁道扇区数
    word heads; //磁头数
    dword hidden_sectors; //隐藏扇区数
    byte drive_no; //驱动器号
    byte boot_sig; //boot标志
    dword volumn_id; //卷号
    word version; //版本号
    dword root_directory_start_clus; //根目录区起始簇号
    word fsinfo_sectors; //FS信息扇区数
    word backup_boot_sectors; //引导备份扇区数
    char volumn_label[11 + 1]; //卷标
    char file_system[8 + 1]; //文件系统
    //Extension
    dword fat1_start; //fat1起始
    dword data_start; //数据区起始
    void *root_directory; //根目录数据
} FAT32_CONTEXT;

#define FAT32_CONTEXT_MAGIC (0x93186A8E)
#define FAT32_FILE_ENTRY_SIZE (0x20)
#define FAT32_FILE_ENTRIES_PER_SECTOR (0x200 / FAT32_FILE_ENTRY_SIZE)
#define FAT32_ENTRY_SIZE (4)
#define FAT32_ENTRIES_PER_SECTOR (0x200 / FAT32_ENTRY_SIZE)

PRIVATE dword get_fat32_entry(void *context, dword last) {
    FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
    int _sector = (last / FAT32_ENTRIES_PER_SECTOR) + _context->fat1_start;
    int _offset = last % FAT32_ENTRIES_PER_SECTOR;
    void *buffer = malloc(512);
    read_sector(_sector, 1, _context->selector, buffer);
    dword entry = *(((dword*)buffer) + _offset);
    free(buffer);
    return entry;
}

PRIVATE void __loadfile(void *context, dword clus, void *dst) {
    FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
    //起始簇号为2
    while (clus < 0x0FFFFFF0) {
        for (int i = 0 ; i < _context->sectors_per_clus ; i ++) {
            read_sector(_context->data_start + (clus - 2) * _context->sectors_per_clus + i, 1, _context->selector, dst);
            dst += 512;
        }
        clus = get_fat32_entry(context, clus);
    }
}

PUBLIC void *get_context(int selector) {
    void *boot = malloc(512);
    FAT32_CONTEXT *context = malloc(sizeof(FAT32_CONTEXT));
    read_sector(0, 1, selector, boot);

    //MARK
    context->selector = selector;
    context->magic = FAT32_CONTEXT_MAGIC;
    //Boot Sector
    memcpy(context->oem_name, (boot + 0x03), 8);
    context->oem_name[8] = '\0';
    //BPB
    context->bytes_per_sector = *(word*)(boot + 0x0B);
    context->sectors_per_clus = *(byte*)(boot + 0x0D);
    context->reversed_sectors = *(word*)(boot + 0x0E);
    context->num_fats = *(byte*)(boot + 0x10);
    context->total_sectors = *(dword*)(boot + 0x20);
    context->media = *(byte*)(boot + 0x15);
    context->fat_sectors = *(dword*)(boot + 0x24);
    context->sectors_per_track = *(word*)(boot + 0x18);
    context->heads = *(word*)(boot + 0x1A);
    context->hidden_sectors = *(dword*)(boot + 0x1C);
    context->version = *(word*)(boot + 0x2A);
    context->root_directory_start_clus = *(dword*)(boot + 0x2C);
    context->fsinfo_sectors = *(word*)(boot + 0x30);
    context->backup_boot_sectors = *(word*)(boot + 0x32);
    //Ex BPB
    context->drive_no = *(byte*)(boot + 0x40);
    context->boot_sig = *(byte*)(boot + 0x42);
    context->volumn_id = *(dword*)(boot + 0x43);
    memcpy(context->volumn_label, (boot + 0x47), 11);
    context->volumn_label[11] = '\0';
    memcpy(context->file_system, (boot + 0x52), 8);
    context->file_system[8] = '\0';
    //Extension
    context->fat1_start = context->reversed_sectors;
    context->data_start = context->fat1_start + context->num_fats * context->fat_sectors;
    context->root_directory = malloc(8192); //16个扇区
    __loadfile(context, context->root_directory_start_clus, context->root_directory);

    free(boot);
    return context;
}

PUBLIC void terminate_fs_context(void *context) {
    free (((FAT32_CONTEXT*)context)->root_directory);
    free (context);
}

PUBLIC void print_fs_info(void *context) {
    FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
    printf ("OEM Name: %s;  Bytes per Sector : %d;  Sectors per Clus: %d\n",
        _context->oem_name, _context->bytes_per_sector, _context->sectors_per_clus);
    printf ("Reversed Sectors: %#X;  FAT Num: %d; Total Sectors: %#X;  Media: %#2X\n",
        _context->reversed_sectors, _context->num_fats, _context->total_sectors, _context->media);
    printf ("FAT Sectors: %#X;  Sectors per Track: %d;  Heads: %d\n",
        _context->fat_sectors, _context->sectors_per_track, _context->heads);
    printf ("Hidden Sectors: %#X;  Version: %d;  Root Directory Start Clus: %d\n",
        _context->hidden_sectors, _context->version, _context->root_directory_start_clus);
    printf ("FSInfo Sectors: %#X;  Backup Boot Sectors: %#X;  Drive No: %#2X\n",
        _context->fsinfo_sectors, _context->backup_boot_sectors, _context->drive_no);
    printf ("Boot Sig: %#2X;  Vol ID: %#8X;  Vol Label: %s\n",
        _context->boot_sig, _context->volumn_id, _context->volumn_label);
    printf ("File System: %s; FAT1 Start: %#8X\n", _context->file_system, _context->fat1_start);
    printf ("Data Start: %#8X\n", _context->data_start);
}

PRIVATE word get_file_start_clus(void *context, const char *name) {
    if (strlen(name) != 11)
        return -1;
    FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
    char rdname[12] = "";
    rdname[11] = 0;
    for (int i = 0 ; i < 16 * FAT32_ENTRIES_PER_SECTOR ; i ++) {
        memcpy(rdname, _context->root_directory + i * FAT32_ENTRY_SIZE + 0, 11);
        if (! strcmp(name, rdname)) {
            return *(word*)(_context->root_directory + i * FAT32_ENTRY_SIZE + 0x1A);
        }
    }
    return -1;
}

PRIVATE char *convert(const char *name, char *buffer) {
    char *backup = buffer;
    int cnt = 0;
    while (*name != '.') {
        *buffer = toupper(*name);
        buffer ++;
        name ++;
        cnt ++;
    }
    if (cnt > 8) {
        return NULL;
    }
    while (cnt < 8) {
        *buffer = ' ';
        buffer ++;
        cnt ++;
    }
    name ++;
    for (int i = 0 ; i < 3 ; i ++) {
        *buffer = toupper(*name);
        buffer ++;
        name ++;
    }
    *buffer = '\0';
    return backup;
}

PUBLIC bool loadfile(void *context, const char *name, void *dst) {
    FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
    char _name[12];
    if (convert(name, _name) == NULL) {
        return false;
    }
    dword clus = get_file_start_clus(context, _name);
    if (clus == -1) {
        return false;
    }
    __loadfile(_context, clus, dst);
    return true;
}