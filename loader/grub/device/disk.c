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

#include <primlib/logger.h>

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
 */
void send_disk_cmd(disk_t *disk, disk_cmd_t cmd) {
    // 硬盘忙
    if (disk->busy) {
        log_error("Disk is busy!");
        return;
    }

    // 等待硬盘空闲
    while (get_disk_status(disk, IDE_BSY_MASK));

    // 设置硬盘忙
    disk->busy = true;

    b8 device_reg = (
        ((cmd.mode  << 6)  & 0x80) |
        ((cmd.slave << 4)  & 0x10) |
        ((cmd.lba   >> 24) & 0x0F) |
        (0xA0) 
    );

    // 设置设备寄存器
    outb(disk->base + IDE_DEVICE, device_reg);

    // Relax IDE
    for (int i = 0 ; i < 5; i ++) {
        inb(disk->base + IDE_STATUS);
    }

    outb(disk->base2 + IDE_DEVICE_CONTROL, 2); // 使用轮询方式

    outb(disk->base + IDE_FEATURES, cmd.features);

    outb(disk->base + IDE_SECTOR_COUNTER, cmd.sector_counter); // 设置扇区计数

    outb(disk->base + IDE_LBA_LOW ,  cmd.lba        & 0xFF); // 设置LBA
    outb(disk->base + IDE_LBA_MID , (cmd.lba >> 8 ) & 0xFF);
    outb(disk->base + IDE_LBA_HIGH, (cmd.lba >> 16) & 0xFF);

    outb(disk->base + IDE_COMMAND, cmd.command); //发送命令
}


/**
 * @brief 加载硬盘
 * 
 * @param base 基地址
 * @param base2 基地址2
 * @param slave 是否为从盘
 * @return 硬盘 
 */
disk_t *load_disk(int base, int base2, bool slave) {
    // 创建硬盘对象
    disk_t *disk = (disk_t *)malloc(sizeof(disk_t));

    disk->base = base;
    disk->base2 = base2;
    disk->busy = false;

    // 发送IDENTITY命令
    disk_cmd_t cmd = {
        .slave = slave,
        .mode = 0,
        .features = 0,
        .sector_counter = 1,
        .lba = 0,
        .command = ATA_IDENTITY
    };

    send_disk_cmd(disk, cmd);

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

    disk->busy = false;

    return disk;
}