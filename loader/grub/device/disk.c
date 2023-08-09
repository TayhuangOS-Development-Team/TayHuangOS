/**
 * @file disk.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 硬盘
 * @version alpha-1.0.0
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <basec/logger.h>

#include <tay/io.h>
#include <tay/ports.h>

#include <device/disk.h>
#include <libs/capi.h>
#include <string.h>

/**
 * @brief 获取硬盘状态
 * 
 * @param disk 硬盘
 * @param mask 掩码
 * @return 状态
 */
bool GetDiskStatus(Disk *disk, byte mask) {
    return inb(disk->base + IDE_STATUS) & mask;
}

/**
 * @brief 发送命令给硬盘
 * 
 * @param disk 硬盘
 * @param cmd 命令
 * @return true 发送成功
 * @return false 发送失败
 */
bool SendDiskCmd(Disk *disk, DiskCmd cmd) {
    // 硬盘忙
    if (disk->busy) {
        LogError("Disk is busy!");
        return false;
    }
    
    // LBA
    byte lbaLow     =  cmd.lba    & 0xFF;
    byte lbaMid     = (cmd.lba >> 8)  & 0xFF;
    byte lbaHigh    = (cmd.lba >> 16) & 0xFF;
    byte lbaHighest = (cmd.lba >> 24) & 0xF;

    // 等待硬盘空闲
    while (GetDiskStatus(disk, IDE_BSY_MASK));

    // 设置硬盘忙
    disk->busy = true;

    b8 deviceReg = (
        ((cmd.mode    << 6) & 0x40) |
        ((disk->slave << 4) & 0x10) |
        ( lbaHighest        & 0x0F) |
        (0xA0) 
    );

    // 设置设备寄存器
    outb(disk->base + IDE_DEVICE, deviceReg);

    // Relax ATA
    for (int i = 0 ; i < 5; i ++) {
        inb(disk->base + IDE_STATUS);
    }

    outb(disk->base2 + IDE_DEVICE_CONTROL, 2); // 使用轮询方式

    outb(disk->base + IDE_FEATURES, cmd.features);

    outb(disk->base + IDE_SECTCNT, cmd.sectorCnt); // 设置扇区计数

    outb(disk->base + IDE_LBA_LOW , lbaLow); // 设置LBA
    outb(disk->base + IDE_LBA_MID , lbaMid);
    outb(disk->base + IDE_LBA_HIGH, lbaHigh);

    outb(disk->base + IDE_COMMAND, cmd.command); //发送命令

    return true;
}

/**
 * @brief 识别硬盘
 * 
 * @param disk 硬盘
 */
inline static bool IdentityDisk(Disk *disk) {
    // 发送IDENTITY命令
    DiskCmd cmd = {
        .mode = 0,
        .features = 0,
        .sectorCnt = 1,
        .lba = 0,
        .command = ATA_IDENTITY
    };

    if (! SendDiskCmd(disk, cmd)) {
        LogError("Couldn't load disk!");
        return false;
    }

    word buffer[256];

    // 等待DRQ
    while (! GetDiskStatus(disk, IDE_DRQ_MASK));

    // 读取IDENTITY内容
    for (int i = 0 ; i < 256 ; i ++) {
        buffer[i] = inw(disk->base + IDE_DATA);
    }

    // 复制序列号
    for (int i = 0 ; i < 10 ; i ++) {
        word w = buffer[i + 0xA];
        disk->serial[i * 2 + 1] = (char)( w       & 0xFF);
        disk->serial[i * 2 + 0] = (char)((w >> 8) & 0xFF);
    }
    disk->serial[20] = '\0';

    // 复制model
    for (int i = 0 ; i < 20 ; i ++) {
        word w = buffer[i + 0x1B];
        disk->model[i * 2 + 1] = (char)( w       & 0xFF);
        disk->model[i * 2 + 0] = (char)((w >> 8) & 0xFF);
    }
    disk->model[40] = '\0';

    // 获取扇区数
    disk->size = *(dword *)(buffer + 0x3C);

    // 重置忙标志
    disk->busy = false;

    return true;
}

/** MSDOS 分区表偏移*/
#define MSDOS_PARTITION_TABLE_OFFSET (0x1BE)
/** MSDOS 分区项大小*/
#define MSDOS_PARTITION_ENTRY_SIZE  (0x10)

/**
 * @brief 加载分区
 * 
 * @param disk 磁盘
 * @param offset 偏移
 * @param parts 分区数组
 */
void LoadParts(Disk *disk, dword offset, Partition **parts) {
    char buffer[1024];

    // 读取分区表
    ReadDisk(disk, offset, 1, buffer);
    
    // 解析分区表
    for (int i = 0 ; i < 4 ; i ++) {
        // 相关信息
        dword start_lba = *(dword *)(buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 8);
        bool bootable   = *(char *) (buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 0);
        dword cnt   = *(dword *)(buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 12);
        byte sysid      = *(byte *) (buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 4);

        // EMPTY
        if (sysid == SI_EMPTY) {
            // empty entry
            parts[i] = NULL;
            continue;
        }

        // 创建partition_t对象
        parts[i] = (Partition *)malloc(sizeof(Partition));
        memset(parts[i], 0, sizeof(Partition));

        // 初始化该对象
        parts[i]->offset = start_lba;
        parts[i]->absoluteOffset = offset + start_lba;
        parts[i]->bootable = bootable;
        parts[i]->size = cnt;
        parts[i]->sysid = sysid;

        // 拓展分区
        if (sysid == SI_EXTENDED) {
            LoadParts(disk, parts[i]->absoluteOffset, parts[i]->subParts);
        }
    }
}

/**
 * @brief 加载硬盘
 * 
 * @param base 基地址
 * @param base2 基地址2
 * @param slave 是否为从盘
 * @return 硬盘 
 */
Disk *LoadDisk(word base, word base2, bool slave) {
    // 创建硬盘对象
    Disk *disk = (Disk *)malloc(sizeof(Disk));

    disk->base = base;
    disk->base2 = base2;
    disk->slave = slave;
    disk->busy = false;

    if (! IdentityDisk(disk)) {
        return NULL;
    }

    // 加载分区
    LoadParts(disk, 0, disk->primitiveParts);

    return disk;
}

/**
 * @brief 输出日志（分区信息）
 * 
 * @param part 分区
 * @param layer 层次
 */
void LogPart(Partition *part, int layer) {
    // 缩进
    char tabs[32];
    // 至多30层缩进
    layer = layer > 31 ? 31 : layer;
    for (int i = 0 ; i < layer ; i ++) {
        tabs[i] = '\t';
    }
    tabs[layer] = '\0';

    // 打印分区信息
    LogInfo("%s----Partition Info----", tabs);
    LogInfo("%sOffset: %d(Absolute: %d)", tabs, part->offset, part->absoluteOffset);
    LogInfo("%sBootable: %s ; System Id: %#02X", tabs, part->bootable ? "true" : "false", part->sysid);
    LogInfo("%sSize: %d sectors(%dB = %dKB = %dMB)", tabs, part->size, part->size * 512, part->size / 2, part->size / 2048);

    // 拓展分区
    if (part->sysid == SI_EXTENDED) {
        // 打印子分区信息
        LogInfo("%sChildren Partitions:", tabs);
        for (int i = 0 ; i < 4; i ++) {
            if (part->subParts[i] != NULL) {
                LogInfo("%sPartition %d:", tabs, i);
                LogPart(part->subParts[i], layer + 1);
            }
        }
    }
}

/**
 * @brief 输出日志（硬盘信息）
 * 
 * @param disk 硬盘
 */
void LogDisk(Disk *disk) {
    LogInfo("----------Disk Info----------");
    LogInfo("Base: (%#04X, %#04X) ; Busy: %s", disk->base, disk->base2, disk->busy ? "true" : "false");
    LogInfo("Serial: %s", disk->serial);
    LogInfo("Model: %s", disk->model);
    LogInfo("Size: %d sectors(%dB = %dKB = %dMB)", disk->size, disk->size * 512, disk->size / 2, disk->size / 2048);

    LogInfo("Primary Partitions:");

    // 打印分区
    for (int i = 0 ; i < 4; i ++) {
        if (disk->primitiveParts[i] != NULL) {
            LogInfo("Partition %d:", i);
            LogPart(disk->primitiveParts[i], 1);
        }
    }
}

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
bool ReadDisk(Disk *disk, dword lba, dword cnt, void *dst) {
    // 发送IDENTITY命令
    DiskCmd cmd = {
        .mode = 1,
        .features = 0,
        .sectorCnt = cnt,
        .lba = lba,
        .command = 0x20
    };

    // 发送命令
    if (! SendDiskCmd(disk, cmd)) {
        LogError("Couldn't read disk cnt!");
        return false;
    }

    for (int i = 0 ; i < cnt ; i ++) { 
        // 等待DRQ
        while (! GetDiskStatus(disk, IDE_DRQ_MASK));
        
        // 读取
        for (int j = 0 ; j < 256 ; j ++) {
            ((word *) dst)[256 * i + j] = inw(disk->base + IDE_DATA);
        }

        // Relax ATA
        for (int i = 0 ; i < 5; i ++) {
            inb(disk->base + IDE_STATUS);
        }
    }
    
    disk->busy = false;

    return true;
}

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
bool ReadPartition(Disk *disk, Partition *part, dword lba, dword cnt, void *dst) {
    return ReadDisk(disk, part->absoluteOffset + lba, cnt, dst);
}