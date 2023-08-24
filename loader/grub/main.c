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
#include <tay/cr.h>
#include <tay/paging.h>

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

#include <lm/elf_loader.h>

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

    fs->fsFunctionTable->InitIteration(fs, dir, &iter);

    while (fs->fsFunctionTable->Next(fs, iter, file) == VFS_PASSED) {
        LogInfo("%s%s", tabs, file->filename, file->fileSizeInBytes);

        if (file->isDirectory) {
            LogDirectory(fs, file, layer + 1);
        }

        fs->fsFunctionTable->Close(file);
    }

    fs->fsFunctionTable->CloseIteration(fs, iter);
}

static const char *kernelPath = "/TayhuangOS/System/tayKernel.bin";

#define KERNEL_BUFFER (0x500000)

void LogCR(void) {
    LogInfo("--------控制寄存器--------");

    CR0 cr0 = rdcr0();
    CR2 cr2 = rdcr2();
    CR3 cr3 = rdcr3();
    CR4 cr4 = rdcr4();

    LogInfo("CR0: %s %s %s %s %s %s %s %s %s %s %s",
        cr0.PE ? "PE" : "pe",
        cr0.MP ? "MP" : "mp",
        cr0.EM ? "EM" : "em",
        cr0.TS ? "TS" : "ts",
        cr0.ET ? "ET" : "et",
        cr0.NE ? "NE" : "ne",
        cr0.WP ? "WP" : "wp",
        cr0.AM ? "AM" : "am",
        cr0.NW ? "NW" : "nw",
        cr0.CD ? "CD" : "cd",
        cr0.PG ? "PG" : "pg"
    );

    LogInfo("CR2: PFLA: %p", cr2.PFLA);

    LogInfo("CR3: page: %p %s %s",
        (dword)(cr3.pageEntry & CR3_PAGE_ENTRY_MASK),
        cr3.PWT ? "PWT" : "pwt",
        cr3.PCD ? "PCD" : "pcd"
    );

    LogInfo("CR4: page: %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
        cr4.VME        ? "VME"        : "vme",
        cr4.PVI        ? "PVI"        : "pvi",
        cr4.TSD        ? "TSD"        : "tsd",
        cr4.DE         ? "DE"         : "de",
        cr4.PSE        ? "PSE"        : "pse",
        cr4.PAE        ? "PAE"        : "pae",
        cr4.MCE        ? "MCE"        : "mce",
        cr4.PGE        ? "PGE"        : "pge",
        cr4.PCE        ? "PCE"        : "pce",
        cr4.OSFXSR     ? "OSFXSR"     : "osfxsr",
        cr4.OSXMMEXCPT ? "OSXMMEXCPT" : "osxmmexcpt",
        cr4.UMIP       ? "UMIP"       : "umip",
        cr4.LA57       ? "LA57"       : "la57",
        cr4.VMXE       ? "VMXE"       : "vmxe",
        cr4.SMXE       ? "SMXE"       : "smxe",
        cr4.FSGSBASE   ? "FSGSBASE"   : "fsgsbase",
        cr4.PCIDE      ? "PCIDE"      : "pcide",
        cr4.OSXSAVE    ? "OSXSAVE"    : "osxsave",
        cr4.KL         ? "KL"         : "kl",
        cr4.SMEP       ? "SMEP"       : "smep",
        cr4.SMAP       ? "SMAP"       : "smap",
        cr4.PKE        ? "PKE"        : "pke",
        cr4.CET        ? "CET"        : "cet",
        cr4.PKS        ? "PKS"        : "pks",
        cr4.UINTR      ? "UINTR"      : "uintr"
    );
}

/**
 * @brief 入口函数
 *
 */
int main(void) {
    // 加载硬盘
    Disk *disk = LoadDisk(IDE0_BASE, IDE0_BASE2, false);

    // 获取启动扇区
    Partition *bootPartition = NULL;
    for (int i = 0 ; i < 4 ; i ++) {
        Partition *partition = disk->primaryPartitions[i];
        // 跳过空分区
        if (partition == NULL) {
            continue;
        }

        // 可启动
        if (partition->isBootable) {
            bootPartition = partition;
        }
    }

    // 无法找到启动扇区
    if (bootPartition == NULL) {
        LogFatal("找不到启动分区!");
        return -1;
    }

    // 打印扇区信息
    LogDisk(disk);
    LogParition(bootPartition, 0);

    FSData *fs = LoadFS(bootPartition);
    if (fs == NULL) {
        LogFatal("无法识别启动分区的文件系统!");
        return -1;
    }

    // LogDirectory(fs, fs->rootDirectory, 0);

    VFile *kernel = OpenFile(fs, kernelPath);
    if (kernel == NULL) {
        LogFatal("未找到内核!");
        return -1;
    }

    if (fs->fsFunctionTable->Read(kernel, (void *)KERNEL_BUFFER) != VFS_PASSED) {
        LogFatal("读取内核时出现异常!");
        return -1;
    }

    CloseFile(kernel);

    LoadInfo kernelLoadInfo;
    if (! LoadELF((void *)KERNEL_BUFFER, &kernelLoadInfo)) {
        LogFatal("加载内核时出现异常!");
        return -1;
    }

    LogInfo("内核入口点: %p", kernelLoadInfo.programEntrypoint);
    LogInfo("内核区间: %p~%p", kernelLoadInfo.programStartAddress, kernelLoadInfo.programLimitAddress);

    UnloadFS(fs);

    LogCR();

    LogInfo("sizeof(PageEntry4K)=%d", sizeof(PageEntry4K));
    LogInfo("sizeof(PageEntry2M)=%d", sizeof(PageEntry2M));
    LogInfo("sizeof(PageEntry1G)=%d", sizeof(PageEntry1G));
    LogInfo("sizeof(PagingTableEntry)=%d", sizeof(PagingTableEntry));
    LogInfo("sizeof(PTE)=%d", sizeof(PTE));
    LogInfo("sizeof(PDE)=%d", sizeof(PDE));
    LogInfo("sizeof(PDPTE)=%d", sizeof(PDPTE));
    LogInfo("sizeof(PML4E)=%d", sizeof(PML4E));
    LogInfo("sizeof(PML5E)=%d", sizeof(PML5E));

    return 0;
}

/**
 * @brief 启动函数
 *
 */
void setup(void) {
    register int magicNumber __asm__("eax"); //Loader 魔数 存放在eax
    register struct multiboot_tag *multibootInfo __asm__("ebx"); //multiboot info 存放在ebx

    // 设置栈
    asm volatile ("movl $0x1000000, %esp");

    if (magicNumber != MULTIBOOT2_BOOTLOADER_MAGIC) { //魔数不匹配
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