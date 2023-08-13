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
#include <init/handler.h>

#include <stdbool.h>

#include <libs/capi.h>
#include <libs/debug.h>

#include <basec/logger.h>

#include <device/disk.h>

#include <fs/vfs.h>
#include <fs/fat32.h>

/**
 * @brief 初始化文件系统
 *
 */
void InitFS(void) {
    RegisterFS(&FAT32FS);
}

/**
 * @brief 初始化
 *
 */
void Init(void) {
    // Step1. 设置GDT
    InitGDT();

    // Step2. 初始化日志器
    InitSerial();
    InitLogger(WriteSerialStr, "GRUB Loader");

    // Step3. 初始化中断
    InitPIC();
    InitIDT();

    // Step4. 初始化文件系统
    InitFS();
}

/**
 * @brief 结束
 *
 */
void Terminate(void) {
    //  打印堆信息
    LogHeap();

    // 永不退出
    while (true);
}

void LogDirectory(FSData *fs, VFile *dir, int layer) {
    // 缩进
    char tabs[32];
    // 至多30层缩进
    layer = layer > 31 ? 31 : layer;
    for (int i = 0 ; i < layer ; i ++) {
        tabs[i] = ' ';
    }
    tabs[layer] = '\0';

    VFile *file = (VFile *)lmalloc(sizeof(VFile));
    void *iter;

    fs->fs->InitIteration(fs, dir, &iter);

    while (fs->fs->Next(fs, iter, file) == VFS_PASSED) {
        LogInfo("%s%s", tabs, file->name, file->size);

        if (file->isDirectory) {
            LogDirectory(fs, file, layer + 1);
        }

        fs->fs->Close(file);
    }

    fs->fs->CloseIteration(fs, iter);
}

static const char *kernelPath = "/TayhuangOS/System/tayKernel.bin";

#define KERNEL_BUFFER (0x500000)

/**
 * @brief 入口函数
 *
 */
void main(void) {
    // 初始化
    Init();

    // 开中断
    EnableInterrupt();

    // 加载硬盘
    Disk *disk = LoadDisk(IDE0_BASE, IDE0_BASE2, false);

    // 获取启动扇区
    Partition *bootPart = NULL;
    for (int i = 0 ; i < 4 ; i ++) {
        Partition *part = disk->mainParts[i];
        // 跳过空分区
        if (part == NULL) {
            continue;
        }

        // 可启动
        if (part->bootable) {
            bootPart = part;
        }
    }

    // 无法找到启动商区
    if (bootPart == NULL) {
        LogFatal("找不到启动分区!");
        Terminate();
    }

    // 打印扇区信息
    LogDisk(disk);
    LogPart(bootPart, 0);

    FSData *fs = LoadFS(bootPart);
    if (fs == NULL) {
        LogFatal("无法识别启动分区的文件系统!");
        Terminate();
    }

    LogDirectory(fs, fs->root, 0);

    VFile *kernel = OpenFile(fs, kernelPath);
    if (kernel == NULL) {
        LogFatal("未找到内核!");
        Terminate();
    }

    fs->fs->Read(kernel, (void *)KERNEL_BUFFER);
    CloseFile(kernel);

    UnloadFS(fs);

    Terminate();
}