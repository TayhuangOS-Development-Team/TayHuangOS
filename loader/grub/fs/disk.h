/**
 * @file disk.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 
 * @version alpha-1.0.0
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/types.h>
#include <tayhuang/attributes.h>

/**
 * @brief 磁盘(把每个分区视作独立的硬盘)
 * 
 */
typedef struct {
    int base_port;
    bool master;
    int partition;
    bool boot;
    int start_sector;
    int sector_number;
} disk_t;

/**
 * @brief 打开磁盘
 * 
 * @param base_port 磁盘起始端口
 * @param master 是否为主驱动器
 * @param partition 分区
 * @return 磁盘结构体
 */
PUBLIC disk_t *open_disk(int base_port, bool master, int partition);
/**
 * @brief 磁盘是否可用
 * 
 * @param disk 磁盘
 * @return true 可用
 * @return false 不可用
 */
PUBLIC bool is_avaliable(disk_t *disk);
/**
 * @brief 读取扇区
 * 
 * @param disk 磁盘对象
 * @param lba 扇区号
 * @param number 扇区数
 * @param buffer 缓存
 */
PUBLIC void read_sector(disk_t *disk, int lba, int number, void *buffer);
/**
 * @brief 关闭磁盘
 * 
 * @param disk 磁盘
 */
PUBLIC void close_disk(disk_t *disk);