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
 * @brief sysid
 * 
 */
enum sysid_enum {
    SI_EMPTY = 0,
    SI_EXTENDED = 5
};

/**
 * @brief 主分区
 * 
 */
typedef struct __partition_struct__ {
    /** 偏移 */
    dword off;
    /** 绝对偏移(相对硬盘起始位置) */
    dword absoff;
    /** 可启动 */
    bool bootable;
    /** 大小(扇区数) */
    dword size;
    /** SystemId */
    byte sysid;
    /** 子扇区 */
    struct __partition_struct__ *subparts[4];
} partition_t;

/**
 * @brief 硬盘
 * 
 */
typedef struct {
    /** 基址 */
    word base;
    /** 基址2 */
    word base2;
    /** 主/从盘 */
    bool slave;
    /** 忙标志 */
    bool busy;
    /** 序列号 */
    char serial[21];
    /** 模型 */
    char model[41];
    /** 大小(扇区) */
    int size;
    /** 硬盘主扇区(最多4个) */
    partition_t *primparts[4];
} disk_t;

/**
 * @brief 硬盘命令
 * 
 */
typedef struct {
    /** 模式 **/
    b8 mode;
    /** 特征 */
    b8 features;
    /** 扇区计数 */
    b8 sectcnt;
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
 * @return true 发送成功
 * @return false 发送失败
 */
bool send_disk_cmd(disk_t *disk, disk_cmd_t cmd);

/**
 * @brief 加载硬盘
 * 
 * @param base 基地址
 * @param base2 基地址2
 * @param slave 是否为从盘
 * @return 硬盘 
 */
disk_t *load_disk(word base, word base2, bool slave);

/**
 * @brief 加载分区
 * 
 * @param disk 磁盘
 * @param off 偏移
 * @param parts 分区数组
 */
void load_parts(disk_t *disk, dword off, partition_t **parts);

/**
 * @brief 输出日志（分区信息）
 * 
 * @param part 分区
 * @param layer 层次
 */
void log_part(partition_t *part, int layer);

/**
 * @brief 输出日志（硬盘信息）
 * 
 * @param disk 硬盘
 */
void log_disk(disk_t *disk);

/**
 * @brief 读扇区
 * 
 * @param disk 硬盘 
 * @param lba LBA
 * @param cnt 扇区数 
 * @param dst 目标地址
 * @return true 读成功
 * @return false 读失败
 */
bool read_disk_sector(disk_t *disk, dword lba, dword cnt, void *dst);

/**
 * @brief 读扇区
 * 
 * @param disk 硬盘 
 * @param part 分区
 * @param lba LBA
 * @param cnt 扇区数 
 * @param dst 目标地址
 * @return true 读成功
 * @return false 读失败
 */
bool read_part_sector(disk_t *disk, partition_t *part, dword lba, dword cnt, void *dst);