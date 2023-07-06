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

#include <bcl/logger.h>

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
bool get_disk_status(disk_t *disk, byte mask) {
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
bool send_disk_cmd(disk_t *disk, disk_cmd_t cmd) {
    // 硬盘忙
    if (disk->busy) {
        lerror("Disk is busy!");
        return false;
    }
    
    byte lba_low     =  cmd.lba    & 0xFF;
    byte lba_mid     = (cmd.lba >> 8)  & 0xFF;
    byte lba_high    = (cmd.lba >> 16) & 0xFF;
    byte lba_highest = (cmd.lba >> 24) & 0xF;

    // 等待硬盘空闲
    while (get_disk_status(disk, IDE_BSY_MASK));

    // 设置硬盘忙
    disk->busy = true;

    b8 device_reg = (
        ((cmd.mode    << 6) & 0x40) |
        ((disk->slave << 4) & 0x10) |
        ( lba_highest       & 0x0F) |
        (0xA0) 
    );

    // 设置设备寄存器
    outb(disk->base + IDE_DEVICE, device_reg);

    // Relax ATA
    for (int i = 0 ; i < 5; i ++) {
        inb(disk->base + IDE_STATUS);
    }

    outb(disk->base2 + IDE_DEVICE_CONTROL, 2); // 使用轮询方式

    outb(disk->base + IDE_FEATURES, cmd.features);

    outb(disk->base + IDE_SECTCNT, cmd.sectcnt); // 设置扇区计数

    outb(disk->base + IDE_LBA_LOW , lba_low); // 设置LBA
    outb(disk->base + IDE_LBA_MID , lba_mid);
    outb(disk->base + IDE_LBA_HIGH, lba_high);

    outb(disk->base + IDE_COMMAND, cmd.command); //发送命令

    return true;
}

/**
 * @brief 识别硬盘
 * 
 * @param disk 硬盘
 */
inline static bool identity_disk(disk_t *disk) {
    // 发送IDENTITY命令
    disk_cmd_t cmd = {
        .mode = 0,
        .features = 0,
        .sectcnt = 1,
        .lba = 0,
        .command = ATA_IDENTITY
    };

    if (! send_disk_cmd(disk, cmd)) {
        lerror("Couldn't load disk!");
        return false;
    }

    word buffer[256];

    // 等待DRQ
    while (! get_disk_status(disk, IDE_DRQ_MASK));

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
#define MSDOS_PT_OFF (0x1BE)
/** MSDOS 分区项大小*/
#define MSDOS_PE_SZ  (0x10)

/**
 * @brief 加载分区
 * 
 * @param disk 磁盘
 * @param off 偏移
 * @param parts 分区数组
 */
void load_parts(disk_t *disk, dword off, partition_t **parts) {
    char buffer[1024];

    // 读取分区表
    read_disk_sector(disk, off, 1, buffer);
    
    // 解析分区表
    for (int i = 0 ; i < 4 ; i ++) {
        // 相关信息
        dword start_lba = *(dword *)(buffer + MSDOS_PT_OFF + i * MSDOS_PE_SZ + 8);
        bool bootable   = *(char *) (buffer + MSDOS_PT_OFF + i * MSDOS_PE_SZ + 0);
        dword cnt   = *(dword *)(buffer + MSDOS_PT_OFF + i * MSDOS_PE_SZ + 12);
        byte sysid      = *(byte *) (buffer + MSDOS_PT_OFF + i * MSDOS_PE_SZ + 4);

        // EMPTY
        if (sysid == SI_EMPTY) {
            // empty entry
            parts[i] = NULL;
            continue;
        }

        // 创建partition_t对象
        parts[i] = (partition_t *)malloc(sizeof(partition_t));
        memset(parts[i], 0, sizeof(partition_t));

        // 初始化该对象
        parts[i]->off = start_lba;
        parts[i]->absoff = off + start_lba;
        parts[i]->bootable = bootable;
        parts[i]->size = cnt;
        parts[i]->sysid = sysid;

        if (sysid == SI_EXTENDED) {
            load_parts(disk, parts[i]->absoff, parts[i]->subparts);
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
disk_t *load_disk(word base, word base2, bool slave) {
    // 创建硬盘对象
    disk_t *disk = (disk_t *)malloc(sizeof(disk_t));

    disk->base = base;
    disk->base2 = base2;
    disk->slave = slave;
    disk->busy = false;

    if (! identity_disk(disk)) {
        return NULL;
    }

    load_parts(disk, 0, disk->primparts);

    return disk;
}

/**
 * @brief 输出日志（分区信息）
 * 
 * @param part 分区
 * @param layer 层次
 */
void log_part(partition_t *part, int layer) {
    char tabs[32];
    layer = layer > 31 ? 31 : layer;
    for (int i = 0 ; i < layer ; i ++) {
        tabs[i] = '\t';
    }
    tabs[layer] = '\0';

    linfo("%s----Partition Info----", tabs);
    linfo("%soff: %d(Absolute: %d)", tabs, part->off, part->absoff);
    linfo("%sBootable: %s ; System Id: %#02X", tabs, part->bootable ? "true" : "false", part->sysid);
    linfo("%sSize: %d sectors(%dB = %dKB = %dMB)", tabs, part->size, part->size * 512, part->size / 2, part->size / 2048);
    if (part->sysid == SI_EXTENDED) {
        linfo("%sChildren Partitions:", tabs);
        for (int i = 0 ; i < 4; i ++) {
            if (part->subparts[i] != NULL) {
                linfo("%sPartition %d:", tabs, i);
                log_part(part->subparts[i], layer + 1);
            }
        }
    }
}

/**
 * @brief 输出日志（硬盘信息）
 * 
 * @param disk 硬盘
 */
void log_disk(disk_t *disk) {
    linfo("----------Disk Info----------");
    linfo("Base: (%#04X, %#04X) ; Busy: %s", disk->base, disk->base2, disk->busy ? "true" : "false");
    linfo("Serial: %s", disk->serial);
    linfo("Model: %s", disk->model);
    linfo("Size: %d sectors(%dB = %dKB = %dMB)", disk->size, disk->size * 512, disk->size / 2, disk->size / 2048);

    linfo("Primary Partitions:");
    for (int i = 0 ; i < 4; i ++) {
        if (disk->primparts[i] != NULL) {
            linfo("Partition %d:", i);
            log_part(disk->primparts[i], 1);
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
bool read_disk_sector(disk_t *disk, dword lba, dword cnt, void *dst) {
    // 发送IDENTITY命令
    disk_cmd_t cmd = {
        .mode = 1,
        .features = 0,
        .sectcnt = cnt,
        .lba = lba,
        .command = 0x20
    };

    // 发送命令
    if (! send_disk_cmd(disk, cmd)) {
        lerror("Couldn't read disk cnt!");
        return false;
    }

    for (int i = 0 ; i < cnt ; i ++) { 
        // 等待DRQ
        while (! get_disk_status(disk, IDE_DRQ_MASK));
        
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
bool read_part_sector(disk_t *disk, partition_t *part, dword lba, dword cnt, void *dst) {
    return read_disk_sector(disk, part->absoff + lba, cnt, dst);
}