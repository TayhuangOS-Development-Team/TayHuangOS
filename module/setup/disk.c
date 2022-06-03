/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * disk.c
 *
 * 硬盘操作
 *
 */



#include "disk.h"
#include <tool/io.h>
#include <syscall/syscall.h>

#define CMOS_RAM_BASE    (0x70)
#define CMOS_RAM_ADDRESS (CMOS_RAM_BASE + 0)
#define CMOS_RAM_DATA    (CMOS_RAM_BASE + 1)

//0号IDE通道
#define IDE0_BASE             (0x1F0)
#define IDE0_BASE2            (0x3F6)
#define IDE0_DATA             (IDE0_BASE + 0)
#define IDE0_ERROR            (IDE0_BASE + 1)
#define IDE0_FEATURES         (IDE0_BASE + 1)
#define IDE0_SECTOR_COUNTER   (IDE0_BASE + 2)
#define IDE0_LBA_LOW          (IDE0_BASE + 3)
#define IDE0_LBA_MID          (IDE0_BASE + 4)
#define IDE0_LBA_HIGH         (IDE0_BASE + 5)
#define IDE0_DEVICE           (IDE0_BASE + 6)
#define IDE0_STATUS           (IDE0_BASE + 7)
#define IDE0_COMMAND          (IDE0_BASE + 7)
#define IDE0_ALTERNATE_STATUS (IDE0_BASE2 + 0)
#define IDE0_DEVICE_CONTROL   (IDE0_BASE2 + 0)

//1号IDE通道
#define IDE1_BASE             (0x170)
#define IDE1_BASE2            (0x376)
#define IDE1_DATA             (IDE1_BASE + 0)
#define IDE1_ERROR            (IDE1_BASE + 1)
#define IDE1_FEATURES         (IDE1_BASE + 1)
#define IDE1_SECTOR_COUNTER   (IDE1_BASE + 2)
#define IDE1_LBA_LOW          (IDE1_BASE + 3)
#define IDE1_LBA_MID          (IDE1_BASE + 4)
#define IDE1_LBA_HIGH         (IDE1_BASE + 5)
#define IDE1_DEVICE           (IDE1_BASE + 6)
#define IDE1_STATUS           (IDE1_BASE + 7)
#define IDE1_COMMAND          (IDE1_BASE + 7)
#define IDE1_ALTERNATE_STATUS (IDE1_BASE2 + 0)
#define IDE1_DEVICE_CONTROL   (IDE1_BASE2 + 0)

#define MAKE_DEVICE_REG(mode, device, lba) (((mode) << 6) | ((device) << 4) | (lba & 0xF) | 0xA0) //MAKE Device寄存器

#define IDE_MASK (0x1)
#define SLAVE_MASK (0x2)

static inline byte read_cmos(int addr) {
    outb (CMOS_RAM_ADDRESS, 0x80 | addr);
    return inb (CMOS_RAM_DATA);
}

static inline void wait_for_ide0_free(void) { //等待ide0空闲
    while (inb(IDE0_STATUS) & 0x80);
}

static inline void relax_ide0(void) { //延迟
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
    inb (IDE0_STATUS);
}

static inline void wait_for_ide1_free(void) { //等待ide1空闲
    while (inb(IDE1_STATUS) & 0x80);
}

static inline void relax_ide1(void) { //延迟
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
    inb (IDE1_STATUS);
}

void identify_ide0_disk(bool slave, void *dst) { //获取IDE0参数
    outb (IDE0_DEVICE, MAKE_DEVICE_REG(0, slave, 0));
    relax_ide0();
    wait_for_ide0_free();

    outb (IDE0_FEATURES, 0);
    outb (IDE0_DEVICE_CONTROL, 0);
    outb (IDE0_SECTOR_COUNTER, 1);
    outb (IDE0_LBA_LOW, 0);
    outb (IDE0_LBA_MID, 0);
    outb (IDE0_LBA_HIGH, 0);
    outb (IDE0_COMMAND, 0xEC); //0xEC: IDENTIFY

    wait_irq (14);

    for (int i = 0 ; i < 256 ; i ++) {
        *(((word*)dst) + i) = inw(IDE0_DATA); //256字数据
    }
}

void identify_ide1_disk(bool slave, void *dst) { //获取IDE1参数
    outb (IDE1_DEVICE, MAKE_DEVICE_REG(0, slave, 0));
    relax_ide1();
    wait_for_ide1_free();

    outb (IDE1_FEATURES, 0);
    outb (IDE1_DEVICE_CONTROL, 0);
    outb (IDE1_SECTOR_COUNTER, 1);
    outb (IDE1_LBA_LOW, 0);
    outb (IDE1_LBA_MID, 0);
    outb (IDE1_LBA_HIGH, 0);
    outb (IDE1_COMMAND, 0xEC); //0xEC: IDENTIFY

    wait_irq (15);

    for (int i = 0 ; i < 256 ; i ++) {
        *(((word*)dst) + i) = inw(IDE1_DATA); //256字数据
    }
}

void identify_disk(int selector, void *dst) {
    if ((selector & IDE_MASK) == 0) {
        identify_ide0_disk((selector & SLAVE_MASK) != 0, dst);
    }
    else {
        identify_ide1_disk((selector & SLAVE_MASK) != 0, dst);
    }
}

void read_ide0_sector(dword lba, int num, bool slave, void *dst) { //读IDE0的扇区
    outb (IDE0_DEVICE, MAKE_DEVICE_REG(1, slave, lba >> 24));
    relax_ide0();
    wait_for_ide0_free();

    outb (IDE0_DEVICE_CONTROL, 0);
    outb (IDE0_FEATURES, 0);
    outb (IDE0_SECTOR_COUNTER, num);
    outb (IDE0_LBA_LOW, lba & 0xFF);
    outb (IDE0_LBA_MID, (lba >> 8) & 0xFF);
    outb (IDE0_LBA_HIGH, (lba >> 16) & 0xFF);
    outb (IDE0_COMMAND, 0x20); //0x20: READ SECTOR

    wait_irq(14);

    for (int i = 0 ; i < num ; i ++) { //num个扇区
        for (int j = 0 ; j < 256 ; j ++) { //每扇区256个字
            *(((word*)dst) + i * 256 + j) = inw(IDE0_DATA);
        }
    }
}


void read_ide1_sector(dword lba, int num, bool slave, void *dst) { //读IDE1的扇区
    outb (IDE1_DEVICE, MAKE_DEVICE_REG(1, slave, lba >> 24));
    relax_ide1();
    wait_for_ide1_free();
    outb (IDE1_DEVICE_CONTROL, 0);
    outb (IDE1_FEATURES, 0);
    outb (IDE1_SECTOR_COUNTER, num);
    outb (IDE1_LBA_LOW, lba & 0xFF);
    outb (IDE1_LBA_MID, (lba >> 8) & 0xFF);
    outb (IDE1_LBA_HIGH, (lba >> 16) & 0xFF);
    outb (IDE1_COMMAND, 0x20); //0x20: READ SECTOR

    wait_irq(15);

    for (int i = 0 ; i < num ; i ++) { //num个扇区
        for (int j = 0 ; j < 256 ; j ++) { //每扇区256个字
            *(((word*)dst) + i * 256 + j) = inw(IDE1_DATA);
        }
    }
}

void read_sector(dword lba, int num, int selector, void *dst) {
    if ((selector & IDE_MASK) == 0) {
        read_ide0_sector(lba, num, (selector & SLAVE_MASK) != 0, dst);
    }
    else {
        read_ide1_sector(lba, num, (selector & SLAVE_MASK) != 0, dst);
    }
}

void load_module(const char *name, void *addr) {
    
}