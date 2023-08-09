/**
 * @file fat32.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief
 * @version alpha-1.0.0
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <fs/fat32.h>
#include <basec/logger.h>

/**
 * @brief FAT32引导扇区信息
 *
 */
struct Fat32BRStruct {
    /** 跳转指令 (+0x00) */
    word  jmpShortIns;
    /** 空指令 (+0x02) */
    byte  nopIns;
    /** OEM名 (+0x03) */
    char  oemName[8];
    /** 每扇区字节数 (+0x0B) */
    word  bytesPerSector;
    /** 每簇扇区数 (+0x0D) */
    byte  sectorsPerClus;
    /** 保留扇区数 (+0x0E) */
    word  reservedSectors;
    /** fat数 (+0x10) */
    byte  fatsNum;
    /** (+0x11) */
    dword reserved0;
    /** 介质类型 (+0x15) */
    byte  media;
    /** (+0x16) */
    word  reserved1;
    /** 每磁道扇区数    (+0x18) */
    word  sectorsPerTrack;
    /* 磁头数 (+0x1A) */
    word  heads;
    /** 隐藏扇区数 (+0x1C) */
    dword hiddenSectors;
    /** 总扇区数 (+0x20) */
    dword totalSectors;
    /** fat扇区数 (+0x24) */
    dword fatSectors;
    /** (+0x28) */
    word  reserved2;
    /** 版本号 (+0x2A) */
    word  version;
    /** 根目录区起始簇号 (+0x2C) */
    dword rootDirStartClus;
    /** FS信息扇区数 (+0x30) */
    word  fsInfoSectors;
    /** 引导备份扇区数 (+0x32) */
    word  backupBootSectors;
    /** (+0x34) */
    dword reserved3;
    /** (+0x38) */
    dword reserved4;
    /** (+0x3C) */
    dword reserved5;
    /** 驱动器号 (+0x40) */
    byte  driveNo;
    /** (+0x41) */
    byte  reserved6;
    /** boot标志 (+0x42) */
    byte  bootSig;
    /** 卷号 (+0x43) */
    dword volumnId;
    /** 卷标 (+0x47) */
    char  volumnLabel[11];
    /** 文件系统 (+0x52) */
    char  fileSystem[8];
} __attribute__((packed));

/**
 * @brief FAT32引导扇区信息
 *
 */
typedef struct Fat32BRStruct Fat32BootRecord;

/**
 * @brief FAT32 日期
 *
 * 成员使用word类型是为了规避使用byte类型时GCC会报出的警告
 *
 * "offset of packed bit-field 'month' has changed in GCC 4.4"
 *
 */
struct Fat32Date {
    /** 年 */
    word year : 7;
    /** 月 */
    word month : 4;
    /** 日 */
    word day : 5;
} __attribute__((packed));

/**
 * @brief FAT32 时间
 *
 * 成员使用word类型是为了规避使用byte类型时GCC会报出的警告
 *
 * "offset of packed bit-field 'minute' has changed in GCC 4.4"
 *
 */
struct Fat32Time {
    /** 时 */
    word hour: 5;
    /** 分 */
    word minute : 6;
    /** 秒 */
    word second : 5;
} __attribute__((packed));

/**
 * @brief FAT32 文件项
 *
 */
struct Fat32FileEntry {
    /** 名称 */
    char name[8];
    /** 扩展名 */
    char extension[3];
    /** 属性 */
    struct {
        /** 只读 */
        bool readonly  : 1;
        /** 隐藏 */
        bool hidden    : 1;
        /** 系统 */
        bool system    : 1;
        /** 卷标 */
        bool volumnID  : 1;
        /** 目录 */
        bool directory : 1;
        /** 归档 */
        bool archive   : 1;
        /** 保留*/
        byte reserved  : 2;
    } attr;
    /** 保留 */
    byte             reserved;
    /** 创建时间(以10s计) */
    byte             creationTimeTenSeconds;
    /** 创建时间 */
    struct Fat32Time creationTime;
    /** 创建日期 */
    struct Fat32Date creationDate;
    /** 上次访问日期 */
    struct Fat32Date lastAccessedDate;
    /** 起始簇号(高16位) */
    word             startClusHigh;
    /** 上次修改时间 */
    struct Fat32Time lastModificationTime;
    /** 上次修改日期 */
    struct Fat32Date lastModificationDate;
    /** 起始簇号(低16位) */
    word             startClusLow;
    /** 文件大小 */
    dword            fileSize;
} __attribute__((packed));

/** LFN属性 */
#define LFN_ATTR (0xF)
/** LFN序号掩码 */
#define LFN_ORDER_MASK (0x40)

/**
 * @brief FAT32 长文件名项
 *
 */
struct Fat32LFNEntry {
    /** 序号 */
    byte order;
    /** 名称1 */
    word name1[5];
    /** 属性 */
    byte attr;
    /** 类型 */
    byte type;
    /** 校验数 */
    byte chksum;
    /** 名称2 */
    word name2[6];
    /** 保留 */
    word reserved;
    /** 名称3 */
    word name3[2];
} __attribute__((packed));

/**
 * @brief Fat32项
 *
 */
typedef union {
    /** 文件项 */
    struct Fat32FileEntry fileEntry;
    /** 长文件名项 */
    struct Fat32LFNEntry lfnEntry;
} Fat32Entry;

/**
 * @brief 加载文件系统
 *
 * @param part 分区
 * @param data FS数据(在Load外自行Malloc)
 *
 * @return 错误号
 */
VFSErrors Fat32Load(Partition *part, FSData *data) {
    return VFS_UNIMPLEMENTED;
}

/**
 * @brief 卸载文件系统
 *
 * @param data FS数据(在Unload外自行Free)
 *
 * @return 错误号
 */
VFSErrors Fat32Unload(FSData *data) {
    return VFS_UNIMPLEMENTED;
}

/**
 * @brief 遍历文件
 *
 * @param data FS数据
 * @param directory 目录文件
 * @param head 首个文件
 *
 * @return 错误号
 */
VFSErrors Fat32Foreach(FSData *data, VFile *directory, VFile *head) {
    return VFS_UNIMPLEMENTED;
}

/**
 * @brief 关闭文件
 *
 * @param file 文件
 *
 * @return 错误号
 */
VFSErrors Fat32Close(VFile *file) {
    return VFS_UNIMPLEMENTED;
}

/**
 * @brief 读取文件
 *
 * @param file 文件
 * @param target 目标地址
 *
 * @return 错误号
 */
VFSErrors Fat32Read(VFile *file, void *target) {
    return VFS_UNIMPLEMENTED;
}

/**
 * @brief 写文件
 *
 * @param file 文件
 * @param source 源地址
 *
 * @return 错误号
 */
VFSErrors Fat32Write(VFile *file, void *source, size_t size) {
    return VFS_UNIMPLEMENTED;
}


/**
 * @brief FAT32文件系统
 *
 */
FS FAT32FS = {
    .Load    = Fat32Load,
    .Unload  = Fat32Unload,
    .Foreach = Fat32Foreach,
    .Close   = Fat32Close,
    .Read    = Fat32Read,
    .Write   = Fat32Write
};