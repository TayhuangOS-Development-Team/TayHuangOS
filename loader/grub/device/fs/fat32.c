/**
 * @file fat32.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32文件系统
 * @version alpha-1.0.0
 * @date 2023-06-26
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <device/fs/fat32.h>
#include <stddef.h>

/**
 * @brief 识别FAT32文件系统
 * 
 * @param disk 硬盘
 * @param part 分区
 * @param bootsec 启动扇区
 * @return true 是FAT32文件系统
 * @return false 不是FAT32文件系统
 */
bool idenfat32(disk_t *disk, partition_t *part, void *bootsec) {
    
    return false;
}

/**
 * @brief 加载FAT32文件系统
 * 
 * @param disk 硬盘
 * @param part 分区
 * @return 文件系统指针
 * @return NULL 加载失败
 */
void *load_fat32(disk_t *disk, partition_t *part) {
    return NULL;
}

/**
 * @brief 读取FAT32文件
 * 
 * @param disk 硬盘
 * @param part 分区
 * @param fs 文件系统指针
 * @param name 文件名
 * @param dst 目标地址
 * @return true 读取成功
 * @return false 读取失败
 */
bool *read_fat32(disk_t *disk, partition_t *part, void *fs, const char *name, void *dst) {
    return false;
}

/**
 * @brief 卸载fat32文件系统
 * 
 * @param fs 文件系统指针
 */
void unload_fat32(void *fs) {
}