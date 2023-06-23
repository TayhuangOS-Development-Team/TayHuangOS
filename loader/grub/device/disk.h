/**
 * @file disk.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 硬盘
 * @version alpha-1.0.0
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/types.h>

/**
 * @brief 硬盘
 * 
 */
typedef struct {
    /** 基址 */
    int base;
    /** 基址2 */
    int base2;
    /** 忙标志 */
    bool busy;
    /** 序列号 */
    char serial[21];
    /** 模型 */
    char model[41];
} disk_t;

/**
 * @brief 硬盘命令
 * 
 */
typedef struct {
    /** 主/从驱动器 */
    bool slave;
    /** 模式 **/
    b8 mode;
    /** 特征 */
    b8 features;
    /** 扇区计数 */
    b8 sector_counter;
    /** LBA */
    b32 lba;
    /** 命令 */
    b8 command;
} disk_cmd_t;

/** DRQ掩码 */
#define IDE_DRQ_MASK (0x8)
/** BSY掩码 */
#define IDE_BSY_MASK (0x80)

/** 读取 */
#define ATA_READ     (0x20)
/** 识别 */
#define ATA_IDENTITY (0xEC)

/**
 * @brief 获取硬盘状态
 * 
 * @param disk 硬盘
 * @param mask 掩码
 * @return 状态
 */
bool get_disk_status(disk_t *disk, byte mask);

/**
 * @brief 发送命令给硬盘
 * 
 * @param disk 硬盘
 * @param cmd 命令
 */
void send_disk_cmd(disk_t *disk, disk_cmd_t cmd);

/**
 * @brief 加载硬盘
 * 
 * @param base 基地址
 * @param base2 基地址2
 * @param slave 是否为从盘
 * @return 硬盘 
 */
disk_t *load_disk(int base, int base2, bool slave);