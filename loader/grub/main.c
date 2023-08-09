/**
 * @file main.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief Loader主函数
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <tay/types.h>
#include <tay/ports.h>

#include <init/gdt.h>
#include <init/idt.h>

#include <stdbool.h>

#include <libs/capi.h>
#include <libs/debug.h>

#include <basec/logger.h>

#include <device/disk.h>

/**
 * @brief 初始化
 * 
 */
void Init(void) {
    InitGDT();

    InitSerial();
    InitLogger(WriteSerialStr, "GRUB Loader");

    InitPIC();
    InitIDT();
}

/**
 * @brief 结束
 * 
 */
void Terminate(void) {
}

/**
 * @brief 入口函数
 * 
 */
void main(void) {
    Init();

    asm volatile ("sti");
    
    Disk *disk = LoadDisk(IDE0_BASE, IDE0_BASE2, false);

    Partition *bootpart = NULL;
    for (int i = 0 ; i < 4 ; i ++) {
        if (disk->primitiveParts[i] != NULL && disk->primitiveParts[i]->bootable) {
            bootpart = disk->primitiveParts[i];
        }
    }

    if (bootpart == NULL) {
        LogFatal("Couldn't found boot partition!");
        while (true);
    }

    LogPart(bootpart, 0);

    while (true);
    
    Terminate();
}