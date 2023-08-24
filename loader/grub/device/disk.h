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
enum SysId {
    SI_EMPTY = 0,
    SI_EXTENDED = 5
};

struct DiskStruct;

/**
 * @brief 主分区
 *
 */
typedef struct PartitionStruct {
    /** 硬盘 */
    struct DiskStruct *diskPtr;
    /** 偏移 */
    dword paritionOffset;
    /** 绝对偏移(相对硬盘起始位置) */
    dword partitionAbsoluteOffset;
    /** 可启动 */
    bool isBootable;
    /** 大小(扇区数) */
    dword partitionSizeInSectors;
    /** SystemId */
    byte systemID;
    /** 子扇区 */
    struct PartitionStruct *childPartitions[4];
} Partition;

/**
 * @brief 硬盘
 *
 */
typedef struct DiskStruct {
    /** 基址 */
    word diskBasePort;
    /** 基址2 */
    word diskBasePort2;
    /** 主/从盘 */
    bool isSlave;
    /** 忙标志 */
    bool isBusy;
    /** 序列号 */
    char diskSerial[21];
    /** 模型 */
    char diskModel[41];
    /** 大小(扇区) */
    dword diskSizeInSectors;
    /** 硬盘主扇区(最多4个) */
    Partition *primaryPartitions[4];
} Disk;

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
    b8 sectorCnt;
    /** LBA */
    b32 lba;
    /** 命令 */
    b8 command;
} DiskCmd;

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
bool GetDiskStatus(Disk *disk, byte mask);

/**
 * @brief 发送命令给硬盘
 *
 * @param disk 硬盘
 * @param cmd 命令
 * @return true 发送成功
 * @return false 发送失败
 */
bool SendDiskCmd(Disk *disk, DiskCmd cmd);

/**
 * @brief 加载硬盘
 *
 * @param basePort 基地址
 * @param basePort2 基地址2
 * @param isSlave 是否为从盘
 * @return 硬盘
 */
Disk *LoadDisk(word basePort, word basePort2, bool isSlave);

/**
 * @brief 卸载硬盘
 *
 * @param disk 硬盘
 */
void UnloadDisk(Disk *disk);

/**
 * @brief 输出日志（分区信息）
 *
 * @param partition 分区
 * @param layer 层次
 */
void LogParition(Partition *partition, int layer);

/**
 * @brief 输出日志（硬盘信息）
 *
 * @param disk 硬盘
 */
void LogDisk(Disk *disk);

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
bool ReadDisk(Disk *disk, dword lba, dword cnt, void *dst);

/**
 * @brief 读扇区
 *
 * @param partition 分区
 * @param lba LBA
 * @param cnt 扇区数
 * @param dst 目标地址
 * @return true 读成功
 * @return false 读失败
 */
bool ReadPartition(Partition *partition, dword lba, dword cnt, void *dst);