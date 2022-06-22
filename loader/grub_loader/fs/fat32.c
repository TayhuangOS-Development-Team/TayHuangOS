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
 * fat32.c
 *
 * FAT32文件系统
 *
 */



#include "fat32.h"
#include <lheap.h>
#include <disk.h>
#include <printf.h>
#include <ctype.h>
#include <string.h>
#include <tayhuang/partition.h>

typedef struct {
    // 
    int selector; //Selector of Context
    dword magic; //0x93186A8E (MD5 of "File Allocation Table 32"的前32位)
    dword start_lba;
    partition_member partition;
    // 
    struct {
        char oem_name[8]; //oem名
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
        char volumn_label[11]; //卷标
        char file_system[8]; //文件系统
    } infomations;
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

// PRIVATE dword get_fat32_entry(fs_context context, dword last) {
//     FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
//     int _sector = (last / FAT32_ENTRIES_PER_SECTOR) + _context->fat1_start;
//     int _offset = last % FAT32_ENTRIES_PER_SECTOR;
//     void *buffer = malloc(512);
//     read_sector(_sector, 1, _context->selector, buffer);
//     dword entry = *(((dword*)buffer) + _offset);
//     free(buffer);
//     return entry;
// }

// PRIVATE void __load_file(fs_context context, dword clus, void *dst) {
//     FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
//     //起始簇号为2
//     while (clus < 0x0FFFFFF0) {
//         for (int i = 0 ; i < _context->sectors_per_clus ; i ++) {
//             read_sector(_context->data_start + (clus - 2) * _context->sectors_per_clus + i, 1, _context->selector, dst);
//             dst += 512;
//         }
//         putchar('.');
//         clus = get_fat32_entry(context, clus);
//     }
// }

// PUBLIC fs_context *get_context(int partition_id, int disk_selector) {
//     void *boot = malloc(512);
//     FAT32_CONTEXT *context = malloc(sizeof(FAT32_CONTEXT));
//     get_partition(disk_selector, partition_id, &context->partition);
//     read_sector(context->partition.start_lba, 1, disk_selector, boot);

//     //MARK
//     context->selector = disk_selector;
//     context->magic = FAT32_CONTEXT_MAGIC;
    
//     memcpy (&context->infomations, boot, sizeof(context->infomations));

//     //Extension
//     context->fat1_start = context->infomations.reversed_sectors;
//     context->data_start = context->fat1_start + context->infomations.num_fats * context->infomations.fat_sectors;
//     context->root_directory = malloc(8192); //16个扇区
//     __load_file(context, context->infomations.root_directory_start_clus, context->root_directory);

//     free(boot);
//     return context;
// }

// PUBLIC void terminate_fs_context(fs_context context) {
//     free (((FAT32_CONTEXT*)context)->root_directory);
//     free (context);
// }

// PRIVATE word get_file_start_clus(fs_context context, const char *name) {
//     if (strlen(name) != 11)
//         return -1;
//     FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
//     char rdname[12] = "";
//     rdname[11] = 0;
//     for (int i = 0 ; i < 16 * FAT32_ENTRIES_PER_SECTOR ; i ++) {
//         memcpy(rdname, _context->root_directory + i * FAT32_ENTRY_SIZE + 0, 11);
//         if (! strcmp(name, rdname)) {
//             return *(word*)(_context->root_directory + i * FAT32_ENTRY_SIZE + 0x1A);
//         }
//     }
//     return -1;
// }

// PRIVATE char *convert(const char *name, char *buffer) {
//     char *backup = buffer;
//     int cnt = 0;
//     while (*name != '.') {
//         *buffer = toupper(*name);
//         buffer ++;
//         name ++;
//         cnt ++;
//     }
//     if (cnt > 8) {
//         return NULL;
//     }
//     while (cnt < 8) {
//         *buffer = ' ';
//         buffer ++;
//         cnt ++;
//     }
//     name ++;
//     for (int i = 0 ; i < 3 ; i ++) {
//         *buffer = toupper(*name);
//         buffer ++;
//         name ++;
//     }
//     *buffer = '\0';
//     return backup;
// }

// PUBLIC bool load_file(fs_context context, const char *name, void *dst) {
//     FAT32_CONTEXT *_context = (FAT32_CONTEXT*)context;
//     char _name[12];
//     if (convert(name, _name) == NULL) {
//         return false;
//     }
//     dword clus = get_file_start_clus(context, _name);
//     if (clus == -1) {
//         return false;
//     }
//     __load_file(_context, clus, dst);
//     return true;
// }