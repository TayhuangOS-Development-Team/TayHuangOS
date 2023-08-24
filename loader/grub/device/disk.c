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
    return inb(disk->diskBasePort + IDE_STATUS) & mask;
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
    if (disk->isBusy) {
        LogError("硬盘忙!");
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
    disk->isBusy = true;


    // 设置设备寄存器
    b8 deviceReg = (
        ((cmd.mode    << 6) & 0x40) |
        ((disk->isSlave << 4) & 0x10) |
        ( lbaHighest        & 0x0F) |
        (0xA0)
    );

    outb(disk->diskBasePort + IDE_DEVICE, deviceReg);

    // 等待设备寄存器设置完成
    for (int i = 0 ; i < 5; i ++) {
        inb(disk->diskBasePort + IDE_STATUS);
    }

    outb(disk->diskBasePort2 + IDE_DEVICE_CONTROL, 2); // 使用轮询方式

    outb(disk->diskBasePort + IDE_FEATURES, cmd.features);

    outb(disk->diskBasePort + IDE_SECTCNT, cmd.sectorCnt); // 设置扇区计数

    outb(disk->diskBasePort + IDE_LBA_LOW , lbaLow); // 设置LBA
    outb(disk->diskBasePort + IDE_LBA_MID , lbaMid);
    outb(disk->diskBasePort + IDE_LBA_HIGH, lbaHigh);

    outb(disk->diskBasePort + IDE_COMMAND, cmd.command); //发送命令

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

    // 发送命令并判断是否失败
    if (! SendDiskCmd(disk, cmd)) {
        LogError("无法识别硬盘!");
        return false;
    }

    word buffer[256];

    // 等待DRQ
    while (! GetDiskStatus(disk, IDE_DRQ_MASK));

    // 读取IDENTITY内容
    for (int i = 0 ; i < 256 ; i ++) {
        buffer[i] = inw(disk->diskBasePort + IDE_DATA);
    }

    // 复制序列号
    for (int i = 0 ; i < 10 ; i ++) {
        word w = buffer[i + 0xA];
        disk->diskSerial[i * 2 + 1] = (char)( w       & 0xFF);
        disk->diskSerial[i * 2 + 0] = (char)((w >> 8) & 0xFF);
    }
    disk->diskSerial[20] = '\0';

    // 复制model
    for (int i = 0 ; i < 20 ; i ++) {
        word w = buffer[i + 0x1B];
        disk->diskModel[i * 2 + 1] = (char)( w       & 0xFF);
        disk->diskModel[i * 2 + 0] = (char)((w >> 8) & 0xFF);
    }
    disk->diskModel[40] = '\0';

    // 获取扇区数
    disk->diskSizeInSectors = *(dword *)(buffer + 0x3C);

    // 重置忙标志
    disk->isBusy = false;

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
inline static void LoadParts(Disk *disk, dword offset, Partition **parts) {
    char buffer[1024];

    // 读取分区表
    ReadDisk(disk, offset, 1, buffer);

    // 解析分区表
    for (int i = 0 ; i < 4 ; i ++) {
        // 相关信息
        dword startLBA = *(dword *)(buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 8);
        bool isBootable   = *(char *) (buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 0);
        dword partitionSizeInSectors   = *(dword *)(buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 12);
        byte systemID      = *(byte *) (buffer + MSDOS_PARTITION_TABLE_OFFSET + i * MSDOS_PARTITION_ENTRY_SIZE + 4);

        // EMPTY
        if (systemID == SI_EMPTY) {
            // empty entry
            parts[i] = NULL;
            continue;
        }

        // 创建partition_t对象
        parts[i] = (Partition *)lmalloc(sizeof(Partition));
        memset(parts[i], 0, sizeof(Partition));

        // 初始化该对象
        parts[i]->paritionOffset = startLBA;
        parts[i]->partitionAbsoluteOffset = offset + startLBA;
        parts[i]->isBootable = isBootable;
        parts[i]->partitionSizeInSectors = partitionSizeInSectors;
        parts[i]->systemID = systemID;
        parts[i]->diskPtr = disk;

        // 拓展分区
        if (systemID == SI_EXTENDED) {
            LoadParts(disk, parts[i]->partitionAbsoluteOffset, parts[i]->childPartitions);
        }
    }
}

/**
 * @brief 加载硬盘
 *
 * @param basePort 基地址
 * @param basePort2 基地址2
 * @param isSlave 是否为从盘
 * @return 硬盘
 */
Disk *LoadDisk(word basePort, word basePort2, bool isSlave) {
    // 创建硬盘对象
    Disk *disk = (Disk *)lmalloc(sizeof(Disk));

    disk->diskBasePort = basePort;
    disk->diskBasePort2 = basePort2;
    disk->isSlave = isSlave;
    disk->isBusy = false;

    if (! IdentityDisk(disk)) {
        return NULL;
    }

    // 加载分区
    LoadParts(disk, 0, disk->primaryPartitions);

    return disk;
}

/**
 * @brief 卸载分区
 *
 * @param part 分区
 */
static void UnloadPart(Partition *part) {
    // 拓展分区
    if (part->systemID == SI_EXTENDED) {
        // 卸载子分区
        for (int i = 0 ; i  < 4 ; i ++) {
            if (part->childPartitions[i] != NULL) {
                UnloadPart(part->childPartitions[i]);
            }
        }
    }
    lfree(part);
}

/**
 * @brief 卸载硬盘
 *
 * @param disk 硬盘
 */
void UnloadDisk(Disk *disk) {
    for (int i = 0 ; i  < 4 ; i ++) {
        if (disk->primaryPartitions[i] != NULL) {
            UnloadPart(disk->primaryPartitions[i]);
        }
    }
    lfree(disk);
}

/**
 * @brief 输出日志（分区信息）
 *
 * @param partition 分区
 * @param layer 层次
 */
void LogParition(Partition *partition, int layer) {
    // 缩进
    char tabs[32];
    // 至多30层缩进
    layer = layer > 31 ? 31 : layer;
    for (int i = 0 ; i < layer ; i ++) {
        tabs[i] = ' ';
    }
    tabs[layer] = '\0';

    // 打印分区信息
    LogInfo("%s----分区信息----", tabs);
    LogInfo("%s偏移: %d扇区(绝对偏移: %d扇区)", tabs, partition->paritionOffset, partition->partitionAbsoluteOffset);
    LogInfo("%s%s ; 系统ID: %#02X", tabs, partition->isBootable ? "可启动" : "不可启动", partition->systemID);

    qword partitionSizeInBytes = partition->partitionSizeInSectors * 512;
    qword partitionSizeInKB = partitionSizeInBytes / 1024;
    qword partitionSizeInMB = partitionSizeInKB / 1024;
    LogInfo("%s大小: %d 扇区(%dB = %dKB = %dMB)", tabs, partition->partitionSizeInSectors, partitionSizeInBytes, partitionSizeInKB, partitionSizeInMB);

    // 拓展分区
    if (partition->systemID == SI_EXTENDED) {
        // 打印子分区信息
        LogInfo("%s子分区:", tabs);
        for (int i = 0 ; i < 4; i ++) {
            if (partition->childPartitions[i] != NULL) {
                LogInfo("%s分区%d:", tabs, i);
                LogParition(partition->childPartitions[i], layer + 1);
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
    LogInfo("----------硬盘信息----------");
    LogInfo("基址: (%#04X, %#04X) ; %s", disk->diskBasePort, disk->diskBasePort2, disk->isBusy ? "忙" : "空闲");
    LogInfo("序列号: %s", disk->diskSerial);
    LogInfo("模型: %s", disk->diskModel);

    qword diskSizeInBytes = disk->diskSizeInSectors * 512;
    qword diskSizeInKB = diskSizeInBytes / 1024;
    qword diskSizeInMB = diskSizeInKB / 1024;
    LogInfo("大小: %d 扇区(%dB = %dKB = %dMB)", disk->diskSizeInSectors, diskSizeInBytes, diskSizeInKB, diskSizeInMB);

    LogInfo("主分区:");

    // 打印分区
    for (int i = 0 ; i < 4; i ++) {
        if (disk->primaryPartitions[i] != NULL) {
            LogInfo("分区 %d:", i);
            LogParition(disk->primaryPartitions[i], 1);
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
    // 发送READ命令
    DiskCmd cmd = {
        .mode = 1,
        .features = 0,
        .sectorCnt = cnt,
        .lba = lba,
        .command = ATA_READ
    };

    // 发送命令
    if (! SendDiskCmd(disk, cmd)) {
        LogError("无法读取硬盘!");
        return false;
    }

    for (int i = 0 ; i < cnt ; i ++) {
        // 等待DRQ
        while (! GetDiskStatus(disk, IDE_DRQ_MASK));

        // 读取
        for (int j = 0 ; j < 256 ; j ++) {
            ((word *) dst)[256 * i + j] = inw(disk->diskBasePort + IDE_DATA);
        }

        // 硬盘刷新DRQ需要一定时间
        for (int i = 0 ; i < 5; i ++) {
            inb(disk->diskBasePort + IDE_STATUS);
        }
    }

    // 重置忙标志
    disk->isBusy = false;

    return true;
}

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
bool ReadPartition(Partition *partition, dword lba, dword cnt, void *dst) {
    return ReadDisk(partition->diskPtr, partition->partitionAbsoluteOffset + lba, cnt, dst);
}