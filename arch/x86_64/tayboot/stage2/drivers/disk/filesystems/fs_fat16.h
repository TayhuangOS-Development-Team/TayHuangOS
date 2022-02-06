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
 * fs_fat16.h
 *
 * Fat16文件系统函数
 *
 */



#pragma once

#include "../filesystem.h"

typedef struct {
    dword magic; //0xD949FA99 (MD5 of "File Allocation Table 16"的前32位)
    char oem_name[8 + 1]; //oem名
    word bytes_per_sector; //每扇区字节数
    byte sectors_per_clus; //没簇扇区数
    word reversed_sectors; //保留扇区
    byte num_fats; //fat数
    word root_entries; //根目录项
    dword total_sectors; //总扇区数
    byte media; //介质
    word fat_sectors; //fat扇区数
    word sectors_per_track; //每磁道扇区数
    word heads; //磁头数
    dword hidden_sectors; //隐藏扇区数
    byte drive_no; //驱动器号
    byte boot_sig; //boot标志
    dword volumn_id; //卷号
    char volumn_label[11 + 1]; //卷标
    char file_system[8 + 1]; //文件系统
    dword fat1_start; //fat1起始
    dword root_directory_start; //根目录区起始
    dword root_directory_sectors; //根目录区扇区数
    dword data_start; //数据区起始
    word entries_per_sector; //每区文件条目数
} fs_fat16_t, *pfs_fat16;

#define FAT16_ROOT_ENTRY_SIZE (0x20) //FAT16文件条目大小

typedef struct {
    char name[12]; //名字
    struct fat16_file_attribute {
        bool read_only : 1;
        bool hidden : 1;
        bool system : 1;
        bool vol : 1;
        bool directory : 1;
        bool archive : 1;
    } attribute; //属性
    char reversed[10]; //保留
    word latest_modify_time; //最后修改时间
    word latest_modify_date; //最后修改日期
    word first_clus; //起始簇
    int length; //长度
} fat16_file_t, *pfat16_file; //fat16文件

PUBLIC bool chk_is_fat16(addr_t first_sector); //是否是fat16文件系统
PUBLIC void *create_fat16_file_system(addr_t first_sector); //获取文件系统参数
PUBLIC void print_fat16_file_system(const pfs_fat16 fat16_fs); //打印文件系统参数
PUBLIC word get_fat16_entry(word no, pdriver disk); //获取fat16项
PUBLIC void get_fat16_file_info_by_name(const char *filename, pdriver disk, pfat16_file file);
PUBLIC void get_fat16_file_info(word entry_no, pdriver disk, pfat16_file file);
PUBLIC void set_fat16_entry(word no, word nxt, pdriver disk);
PUBLIC void set_fat16_file_info_by_name(const char *filename, pdriver disk, const pfat16_file file);
PUBLIC void set_fat16_file_info(word entry_no, pdriver disk, pfat16_file file);
PUBLIC word find_free_file_entry(pdriver disk);
PUBLIC bool insert_fat16_file_info(pdriver disk, pfat16_file file);
PUBLIC word get_fat16_file_no(const char *filename, pdriver disk);
PUBLIC word append_new_sector(word last, pdriver disk);
PUBLIC word find_free_entry(pdriver disk);