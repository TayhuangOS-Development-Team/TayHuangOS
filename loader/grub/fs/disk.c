/**
 * @file disk.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 磁盘驱动
 * @version alpha-1.0.0
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/disk.h>

#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#include <heap.h>
#include <string.h>

/**
 * @brief DRQ - 数据已准备
 * 
 */
#define DRQ_MASK (0x8)
/**
 * @brief BUST - 忙
 * 
 */
#define BUSY_MASK (0x80)

/**
 * @brief 探测磁盘命令
 * 
 */
#define ATA_IDENTIFY (0xEC)
/**
 * @brief 读取命令
 * 
 */
#define ATA_READ (0x20)

#define MSDOS_PT_OFFSET (0x01BE)

/**
 * @brief 等待一会儿
 * 
 * @param base1 起始端口
 */
INLINE void relax_ide(int base1) {
    inb (base1 + IDE_STATUS);
    inb (base1 + IDE_STATUS);
    inb (base1 + IDE_STATUS);
    inb (base1 + IDE_STATUS);
    inb (base1 + IDE_STATUS);
}
/**
 * @brief 等待DRQ
 * 
 * @param base1 起始端口
 */
INLINE void wait_drq(int base1) {
    while (! (inb(base1 + IDE_STATUS) & DRQ_MASK));
}

/**
 * @brief 等待 !BUSY
 * 
 * @param base1 起始端口
 */
INLINE void wait_for_free(int base1) {
    while (inb(base1 + IDE_STATUS) & BUSY_MASK);
}

/**
 * @brief 设备寄存器
 * 
 */
#define MAKE_DEVICE_REG(mode, device, lba) (\
    ((mode) << 6) |\
    ((device) << 4) |\
    (lba & 0xF) |\
    (0xA0)\
    )

/**
 * @brief 获取硬盘信息
 * 
 * @param base1 起始端口1
 * @param base2 起始端口2
 * @param slave 主从位
 * @param buffer 缓存
 */
PRIVATE void __identify_disk__(int base1, int base2, bool slave, void *buffer) {
    // 设置设备寄存器
    outb (base1 + IDE0_DEVICE, MAKE_DEVICE_REG(0, slave, 0));
    // 等待
    relax_ide(base1);
    // 等待 !BUSY
    wait_for_free(base1);
    // 不启用中断
    outb (base2 + IDE_DEVICE_CONTROL, 2);
    // 不使用特性
    outb (base1 + IDE_FEATURES, 0);
    // 读取1个扇区大小的数据
    outb (base1 + IDE_SECTOR_COUNTER, 1);
    // 读取的LBA设置为0
    outb (base1 + IDE_LBA_LOW, 0);
    outb (base1 + IDE_LBA_MID, 0);
    outb (base1 + IDE_LBA_HIGH, 0);
    //发送命令
    outb (base1 + IDE_COMMAND, ATA_IDENTIFY);

    // 等待drq
    wait_drq(base1);

    //复制数据
    word *dst = (word *)buffer;
    //读取256字=512字节
    for (int i = 0 ; i < 256 ; i ++) {
        *(dst + i) = inw(IDE0_DATA);
    }
}

/**
 * @brief 读取扇区
 * 
 * @param base1 起始端口1
 * @param base2 起始端口2
 * @param slave 主从位
 * @param lba 扇区号
 * @param number 扇区数
 * @param buffer 缓存
 */
PRIVATE void __read_sector__(int base1, int base2, bool slave, int lba, int number, void *buffer) {
    byte lba_low = lba & 0xFF;
    byte lba_mid = (lba >> 8) & 0xFF;
    byte lba_high = (lba >> 16) & 0xFF;
    byte lba_highest = (lba >> 24) & 0xF;

    // 设置设备寄存器
    outb (base1 + IDE0_DEVICE, MAKE_DEVICE_REG(1, slave, lba_highest));
    // 等待
    relax_ide(base1);
    // 等待 !BUSY
    wait_for_free(base1);
    // 不启用中断
    outb (base2 + IDE_DEVICE_CONTROL, 2);
    // 不使用特性
    outb (base1 + IDE_FEATURES, 0);
    // 读取1个扇区大小的数据
    outb (base1 + IDE_SECTOR_COUNTER, number);
    // 设置LBA
    outb (base1 + IDE_LBA_LOW, lba_low);
    outb (base1 + IDE_LBA_MID, lba_mid);
    outb (base1 + IDE_LBA_HIGH, lba_high);
    //发送命令
    outb (base1 + IDE_COMMAND, ATA_READ);

    // 等待drq
    wait_drq(base1);

    //复制数据
    word *dst = (word *)buffer;
    //读取num个扇区
    for (int i = 0 ; i < number ; i ++) {
        //读取前等待DRQ
        wait_drq(base1);
        
        //每次读取256字=512字节
        for (int j = 0 ; j < 256 ; j ++) {
            *(dst + i * 256 + j) = inw(IDE0_DATA);
        }

        relax_ide(base1);
    }
}

PUBLIC disk_t *open_disk(int base1, int base2, bool slave, int partition) {
    // 创建disk对象
    disk_t *disk = malloc(sizeof(disk_t));
    // 设置基础信息
    disk->base1 = base1;
    disk->base2 = base2;
    disk->slave = slave;
    disk->partition = partition;
    // 缓冲
    char _buffer[512];
    void *buffer = _buffer;
    // 探测
    __identify_disk__(base1, base2, slave, buffer);
    // 复制序列号
    for (int i = 0 ; i < 10 ; i ++) {
        disk->serial[i * 2 + 0] = *(char *)(buffer + i * 2 + 0x15);
        disk->serial[i * 2 + 1] = *(char *)(buffer + i * 2 + 0x14);
    }
    disk->serial[20] = '\0';
    // 复制型号
    for (int i = 0 ; i < 20 ; i ++) {
        disk->model[i * 2 + 0] = *(char *)(buffer + i * 2 + 0x37);
        disk->model[i * 2 + 1] = *(char *)(buffer + i * 2 + 0x36);
    }
    disk->model[40] = '\0';
    // 获得硬盘扇区数
    disk->disk_sector_number = *(dword *)(buffer + 0x78);
    // 获取MBR
    __read_sector__(base1, base2, slave, 0, 1, buffer);
    // 表项偏移
    int offset = partition * 0x10 + MSDOS_PT_OFFSET;
    // 设置分区信息
    disk->bootable = *(byte *)(buffer + offset + 0) == 0x80;
    disk->system_id = *(byte *)(buffer + offset + 4);
    disk->start_sector = *(dword *)(buffer + offset + 8);
    disk->sector_number = *(dword *)(buffer + offset + 12);
    return disk;
}

PUBLIC void close_disk(disk_t *disk) {
    free (disk);
}

PUBLIC bool is_avaliable(disk_t *disk) {
    return disk->sector_number != 0;
}

PUBLIC void read_sector(disk_t *disk, int lba, int number, void *buffer) {
    __read_sector__(disk->base1, disk->base2, disk->slave, disk->start_sector + lba, number, buffer);
}