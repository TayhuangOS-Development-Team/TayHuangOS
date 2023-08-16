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

#include <basec/logger.h>
#include <stdbool.h>
#include <multiboot2.h>

#include <init/gdt.h>
#include <init/idt.h>
#include <init/handler.h>

#include <libs/capi.h>
#include <libs/debug.h>

#include <device/disk.h>

#include <fs/vfs.h>
#include <fs/fat32.h>

#include <lm/elfLoader.h>

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

    // 开中断
    EnableInterrupt();
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
int main(void) {
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

    // 无法找到启动扇区
    if (bootPart == NULL) {
        LogFatal("找不到启动分区!");
        return -1;
    }

    // 打印扇区信息
    LogDisk(disk);
    LogPart(bootPart, 0);

    FSData *fs = LoadFS(bootPart);
    if (fs == NULL) {
        LogFatal("无法识别启动分区的文件系统!");
        return -1;
    }

    LogDirectory(fs, fs->root, 0);

    VFile *kernel = OpenFile(fs, kernelPath);
    if (kernel == NULL) {
        LogFatal("未找到内核!");
        return -1;
    }

    if (fs->fs->Read(kernel, (void *)KERNEL_BUFFER) != VFS_PASSED) {
        LogFatal("读取内核时出现异常!");
        return -1;
    }

    CloseFile(kernel);

    LoadInfo kernelLoadInfo;
    if (! LoadELF((void *)KERNEL_BUFFER, &kernelLoadInfo)) {
        LogFatal("加载内核时出现异常!");
        return -1;
    }

    LogInfo("内核入口点: %p", kernelLoadInfo.entrypoint);
    LogInfo("内核区间: %p~%p", kernelLoadInfo.start, kernelLoadInfo.limit);

    UnloadFS(fs);

    return 0;
}

/**
 * @brief 启动函数
 *
 */
void setup(void) {
    register int magic __asm__("eax"); //Loader 魔数 存放在eax
    register struct multiboot_tag *multibootInfo __asm__("ebx"); //multiboot info 存放在ebx

    // 设置栈
    asm volatile ("movl $0x1008000, %esp");

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) { //魔数不匹配
        while (true);
    }

    // 初始化
    Init();

    // 主函数
    int ret = main();
    if (ret != 0) {
        LogFatal("加载器发生错误!");
    }

    // 收尾
    Terminate();
}