/**
 * @file disk.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 磁盘驱动
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
    /**
     * @brief 磁盘起始端口
     * 
     */
    int base1;
    /**
     * @brief 磁盘起始端口2
     * 
     */
    int base2;
    /**
     * @brief 序列号
     * 
     */
    char serial[21];
    /**
     * @brief 型号
     * 
     */
    char model[41];
    /**
     * @brief 磁盘扇区总数
     * 
     */
    int disk_sector_number;
    /**
     * @brief 主/从磁盘
     * 
     */
    bool slave;
    //分区相关
    /**
     * @brief 扇区
     * 
     */
    int partition;
    /**
     * @brief 是否可启动
     * 
     */
    bool bootable;
    /**
     * @brief 起始扇区号
     * 
     */
    int start_sector;
    /**
     * @brief 扇区总数
     * 
     */
    int sector_number;
    /**
     * @brief 分区类型
     * 
     */
    byte system_id;
} disk_t;

/**
 * @brief 打开磁盘
 * 
 * @param base1 磁盘起始端口
 * @param base2 磁盘起始端口2
 * @param slave 是否为从驱动器
 * @param partition 分区
 * @return 磁盘结构体
 */
PUBLIC disk_t *open_disk(int base1, int base2, bool slave, int partition);
/**
 * @brief 关闭磁盘
 * 
 * @param disk 磁盘
 */
PUBLIC void close_disk(disk_t *disk);
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