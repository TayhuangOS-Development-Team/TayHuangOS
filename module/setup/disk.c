/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * disk.c
 *
 * 硬盘操作函数
 *
 */



#include <disk.h>

#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#include <memory/malloc.h>
#include <syscall/syscall.h>
#include <syscall/ipc.h>

#include <string.h>

#include <debug/logging.h>


#define IDE_MASK (0x1)
#define SLAVE_MASK (0x2)

#define MAKE_DEVICE_REG(mode, device, lba) (((mode) << 6) | ((device) << 4) | (lba & 0xF) | 0xA0) //MAKE Device寄存器

#define ATA_READ (0x20)
#define ATA_IDENTIFY (0xEC)

PRIVATE inline void wait_for_ide0_free(void) { //等待ide0空闲
    while (inb(IDE0_STATUS) & 0x80);
}

PRIVATE inline void relax_ide0(void) { //延迟
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
}

PRIVATE inline void wait_for_ide1_free(void) { //等待ide1空闲
    while (inb(IDE1_STATUS) & 0x80);
}

PRIVATE inline void relax_ide1(void) { //延迟
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
}

PRIVATE void identify_ide0_disk(bool slave, void *dst) { //获取IDE0参数
    set_allow(-2);

    outb (IDE0_DEVICE, MAKE_DEVICE_REG(0, slave, 0));
    relax_ide0();
    wait_for_ide0_free();
    outb (IDE0_DEVICE_CONTROL, 0);
    outb (IDE0_FEATURES, 0);
    outb (IDE0_SECTOR_COUNTER, 1);
    outb (IDE0_LBA_LOW, 0);
    outb (IDE0_LBA_MID, 0);
    outb (IDE0_LBA_HIGH, 0);
    outb (IDE0_COMMAND, ATA_IDENTIFY);

    check_ipc();
    int irq;
    recv_msg(&irq);
    if (irq != 14) {
        lerror ("wrong irq!");
        return;
    }

    for (int i = 0 ; i < 256 ; i ++) {
        *(((word*)dst) + i) = inw(IDE0_DATA); //256字数据
    }
}

PRIVATE void identify_ide1_disk(bool slave, void *dst) { //获取IDE1参数
    set_allow(-2);

    outb (IDE1_DEVICE, MAKE_DEVICE_REG(0, slave, 0));
    relax_ide1();
    wait_for_ide1_free();
    outb (IDE1_DEVICE_CONTROL, 2);
    outb (IDE1_FEATURES, 0);
    outb (IDE1_SECTOR_COUNTER, 1);
    outb (IDE1_LBA_LOW, 0);
    outb (IDE1_LBA_MID, 0);
    outb (IDE1_LBA_HIGH, 0);
    outb (IDE1_COMMAND, ATA_IDENTIFY);
    
    check_ipc();
    int irq;
    recv_msg(&irq);
    if (irq != 15) {
        lerror ("wrong irq!");
        return;
    }

    for (int i = 0 ; i < 256 ; i ++) {
        *(((word*)dst) + i) = inw(IDE1_DATA); //256字数据
    }
}

PUBLIC void identify_disk(int selector, void *dst) {
    if ((selector & IDE_MASK) == 0) {
        identify_ide0_disk((selector & SLAVE_MASK) != 0, dst);
    }
    else {
        identify_ide1_disk((selector & SLAVE_MASK) != 0, dst);
    }
}

PRIVATE void read_ide0_sector(dword lba, bool slave, void *dst) { //读IDE0的扇区
    set_allow(-2);

    outb (IDE0_DEVICE, MAKE_DEVICE_REG(1, slave, lba >> 24));
    relax_ide0();
    wait_for_ide0_free();
    outb (IDE0_DEVICE_CONTROL, 0);
    outb (IDE0_FEATURES, 0);
    outb (IDE0_SECTOR_COUNTER, 1);
    outb (IDE0_LBA_LOW, lba & 0xFF);
    outb (IDE0_LBA_MID, (lba >> 8) & 0xFF);
    outb (IDE0_LBA_HIGH, (lba >> 16) & 0xFF);
    outb (IDE0_COMMAND, ATA_READ);

    check_ipc();
    int irq;
    recv_msg(&irq);
    if (irq != 14) {
        lerror ("wrong irq!");
        return;
    }

    for (int i = 0 ; i < 256 ; i ++) { //每扇区256个字
        *(((word*)dst) + i) = inw(IDE0_DATA);
    }
}


PRIVATE void read_ide1_sector(dword lba, bool slave, void *dst) { //读IDE1的扇区
    set_allow(-2);

    outb (IDE1_DEVICE, MAKE_DEVICE_REG(1, slave, lba >> 24));
    relax_ide1();
    wait_for_ide1_free();
    outb (IDE1_DEVICE_CONTROL, 2);
    outb (IDE1_FEATURES, 0);
    outb (IDE1_SECTOR_COUNTER, 1);
    outb (IDE1_LBA_LOW, lba & 0xFF);
    outb (IDE1_LBA_MID, (lba >> 8) & 0xFF);
    outb (IDE1_LBA_HIGH, (lba >> 16) & 0xFF);
    outb (IDE1_COMMAND, ATA_READ);

    check_ipc();
    int irq;
    recv_msg(&irq);
    if (irq != 15) {
        lerror ("wrong irq!");
        return;
    }

    for (int i = 0 ; i < 256 ; i ++) { //每扇区256个字
        *(((word*)dst) + i) = inw(IDE1_DATA);
    }
}

PUBLIC void read_sector(dword lba, int num, int selector, void *dst) {
    if ((selector & IDE_MASK) == 0) {
        for (int i = 0 ; i < num ; i ++) {
            read_ide0_sector(lba + i, (selector & SLAVE_MASK) != 0, dst + i * 512);
        }
    }
    else {
        for (int i = 0 ; i < num ; i ++) {
            read_ide1_sector(lba + i, (selector & SLAVE_MASK) != 0, dst + i * 512);
        }
    }
}

PUBLIC void get_partition(int selector, int number, partition_info *member) {
    void *buffer = malloc(512);

    read_sector(0, 1, selector, buffer);

    void *table_start = buffer + PARTITION_TABLE_OFFSET;
    int member_sz = sizeof(partition_info);

    memcpy(member, table_start + number * member_sz, member_sz);

    free (buffer);
}