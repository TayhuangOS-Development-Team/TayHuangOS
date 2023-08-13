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
#include <libs/capi.h>
#include <basec/logger.h>
#include <string.h>

/**
 * @brief FAT32引导扇区信息
 *
 */
struct FAT32BRStruct {
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
typedef struct FAT32BRStruct FAT32BootRecord;

/**
 * @brief FAT32 日期
 *
 * 成员使用word类型是为了规避使用byte类型时GCC会报出的警告
 *
 * "offset of packed bit-field 'month' has changed in GCC 4.4"
 *
 */
struct FAT32Date {
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
struct FAT32Time {
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
struct FAT32FileEntry {
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
    struct FAT32Time creationTime;
    /** 创建日期 */
    struct FAT32Date creationDate;
    /** 上次访问日期 */
    struct FAT32Date lastAccessedDate;
    /** 起始簇号(高16位) */
    word             startClusHigh;
    /** 上次修改时间 */
    struct FAT32Time lastModificationTime;
    /** 上次修改日期 */
    struct FAT32Date lastModificationDate;
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
 * 名称中每个word只需取最低字节即可
 * 例如'你好' unicode码为
 * e4bda0 e5a5bd
 * 在FAT32中存储为(小端序)
 * e400 bd00 a000    4500 a500 bd00
 */
struct FAT32LFNEntry {
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
 * @brief FAT32项
 *
 */
typedef union {
    /** 文件项 */
    struct FAT32FileEntry fileEntry;
    /** 长文件名项 */
    struct FAT32LFNEntry lfnEntry;
} FAT32Entry;

/** 缓存区大小 */
#define FAT_CACHE_SIZE (2 * 4096)
/** 缓存不可用 */
#define CACHE_NOT_VALID (0xFFFFFFFF)
/** 终止FAT值 */
#define FINAL_FAT (0x0FFFFFF0)

typedef struct {
    /** FAT缓存起始扇区 */
    dword fatCacheStart;
    /** FAT区缓存 */
    byte fatCache[FAT_CACHE_SIZE];
} FAT32FATCache;

/**
 * @brief FAT32文件系统数据
 *
 */
typedef struct {
    /** 引导记录 */
    FAT32BootRecord bootRecord;
    /** FAT区起始 */
    dword fatStart;
    /** 数据区起始 */
    dword dataStart;
    /** FAT区缓存 */
    FAT32FATCache fatCache;
} FAT32FSData;

/**
 * @brief FAT32文件数据
 *
 */
typedef struct {
    /** FAT32文件项 */
    struct FAT32FileEntry entry;
    /** 起始簇号 */
    dword startClus;
} FAT32FileData;

/**
 * @brief FAT32迭代器
 */
typedef struct {
    /** 目录 */
    VFile *dir;
    /** 上次偏移 */
    dword lastOffset;
} FAT32Iteration;

typedef dword FAT32FATEntry;

/** 为了方便 */
#define __DATA__ ((FAT32FSData *)data->data)
#define __BR__ (&__DATA__->bootRecord)
#define __CACHE__ (&__DATA__->fatCache)
#define __BPS__ (__BR__->bytesPerSector)
#define __SPC__ (__BR__->sectorsPerClus)
#define __ROOT__ (data->root)
#define __FILEDATA__ ((FAT32FileData *)file->data)

/**
 * @brief 获取数据所在扇区
 *
 * @param data FS数据
 * @param clus 簇号
 * @return 数据所在扇区
 */
inline static dword GetDataSector(FSData *data, dword clus) {
    return __DATA__->dataStart + (clus - 2) * __SPC__;
}

/**
 * @brief 更新FAT缓存
 *
 * @param data FS数据
 * @param fatNo fat项号
 *
 * @return FAT项在缓存中的偏移
 */
inline static dword UpdateFATCache(FSData *data, dword fatNo) {
    // 计算缓存所存储的扇区数
    const dword cacheSectors = FAT_CACHE_SIZE / __BPS__;
    // 计算FAT项所处扇区
    const dword fatSector =  fatNo * sizeof(FAT32FATEntry) / __BPS__;

    // 缓存不可用
    if (
        __CACHE__->fatCacheStart == CACHE_NOT_VALID ||
        fatSector < __CACHE__->fatCacheStart ||
        fatSector >= __CACHE__->fatCacheStart + cacheSectors
    ) {
        // 更新缓存
        __CACHE__->fatCacheStart = fatSector;
        if (! ReadPartition(data->part, __CACHE__->fatCacheStart + __DATA__->fatStart, cacheSectors, __CACHE__->fatCache)) {
            LogError("无法更新FAT缓存!");
            return CACHE_NOT_VALID;
        }
    }

    // 返回偏移
    return fatNo * sizeof(FAT32FATEntry) - __CACHE__->fatCacheStart * __BPS__;
}

inline static void InitFile(FSData *data, VFile *file) {
    file->fs = data;
    file->disk = data->disk;
    file->part = data->part;

    file->data = (void *)lmalloc(sizeof(FAT32FileData));
    memset(file->data, 0, sizeof(sizeof(FAT32FileData)));

    file->cache = NULL;
    file->size = 0;

    file->isDirectory = file->isRoot = false;
}

/**
 * @brief 加载文件数据
 *
 * @param data 数据
 * @param startClus 起始簇号
 * @param cache 文件缓存
 */
inline static void __LoadFileData__(FSData *data, dword startClus, void *cache) {
    dword cnt = 0;

    // 设置起始簇
    dword clus = startClus;
    while (clus <= FINAL_FAT) {
        // 读簇
        ReadPartition(
            data->part,
            __DATA__->dataStart + (clus - 2) * __SPC__,
            __SPC__,
            cache + cnt * __SPC__ * __BPS__
        );

        cnt += 1;

        // 获取下个簇
        dword off = UpdateFATCache(data, clus);
        clus = *(dword *)(&__CACHE__->fatCache[off]);
    }
}

/**
 * @brief 更新文件缓存
 *
 * @param data FS数据
 * @param file 文件
 */
inline static void UpdateFileCache(FSData *data, VFile *file) {
    if (file->cache == NULL) {
        // 计算缓存大小
        dword cacheSize = file->size;
        dword fixedSize = (cacheSize / __BPS__) * __BPS__;
        if (cacheSize % __BPS__ != 0) {
            fixedSize += __BPS__;
        }

        file->cache = (void *)lmalloc(fixedSize);
        __LoadFileData__(data, __FILEDATA__->startClus, file->cache);
    }
}

/**
 * @brief 获取目录大小
 *
 * @param data FS数据
 * @param startClus 目录起始簇
 * @return 根目录大小
 */
inline static dword GetDirectorySize(FSData *data, dword startClus) {
    dword sectorNum = 0;

    dword clus = startClus;
    while (clus <= FINAL_FAT) {
        sectorNum += 1;

        dword off = UpdateFATCache(data, clus);
        clus = *(dword *)(&__CACHE__->fatCache[off]);
    }

    return sectorNum * __BPS__;
}

/**
 * @brief 加载文件系统
 *
 * @param part 分区
 * @param data FS数据(在Load外自行Malloc)
 *
 * @return 错误号
 */
VFSErrors FAT32Load(Partition *part, FSData *data) {
    // 设置data
    data->data = (void *)lmalloc(sizeof(FAT32FSData));

    // 读取引导扇区
    char boot[4096];
    ReadPartition(part, 0, 1, boot);

    // 复制Boot Record
    memcpy(__BR__, boot, sizeof(FAT32BootRecord));

    // 检测是否是正确的文件系统
    bool flag = (memcmp(__BR__->fileSystem, "FAT32   ", 8) != 0);

    // 非零数据为零
    flag |= (__BPS__ == 0);
    flag |= (__SPC__ == 0);
    flag |= (__BR__->fatsNum == 0);
    flag |= (__BR__->sectorsPerTrack == 0);
    flag |= (__BR__->heads == 0);
    flag |= (__BR__->totalSectors == 0);
    flag |= (__BR__->fatSectors == 0);

    // BootRecord错误
    if (flag) {
        // 释放Data项
        lfree(data->data);
        return VFS_INVALID;
    }

    __DATA__->fatStart = __BR__->reservedSectors;
    __DATA__->dataStart = __DATA__->fatStart + __BR__->fatsNum * __BR__->fatSectors;
    __DATA__->fatCache.fatCacheStart = CACHE_NOT_VALID;

    // 加载Root
    dword rootDirSize = GetDirectorySize(data, __BR__->rootDirStartClus);

    data->root = (VFile *)lmalloc(sizeof(VFile));
    InitFile(data, data->root);

    data->root->name = (char *)lmalloc(2);
    data->root->name[0] = '/';
    data->root->name[1] = '\0';

    ((FAT32FileData *)data->root->data)->startClus = __BR__->rootDirStartClus;
    data->root->size = rootDirSize;

    data->root->isDirectory = data->root->isRoot = true;

    return VFS_PASSED;
}

/**
 * @brief 卸载文件系统
 *
 * @param data FS数据(在Unload外自行Free)
 *
 * @return 错误号
 */
VFSErrors FAT32Unload(FSData *data) {
    // 关闭根目录文件
    VFSErrors errcode = FAT32Close(data->root);
    if (errcode != VFS_PASSED) {
        return errcode;
    }

    // 释放额外信息
    lfree(data->data);
    lfree(data->root);

    // 系统已卸载
    return VFS_PASSED;
}

/**
 * @brief 初始化迭代器
 *
 * @param data FS数据
 * @param directory 目录文件
 * @param iter 迭代器
 *
 * @return 错误号
 */
VFSErrors FAT32InitIteration(FSData *data, VFile *directory, void **iter) {
    if (! directory->isDirectory) {
        LogError("将非目录文件作为目录传递给InitIteration函数");
        return VFS_INVALID;
    }

    UpdateFileCache(data, directory);

    // 初始化迭代器
    FAT32Iteration *iteration = (FAT32Iteration *)lmalloc(sizeof(FAT32Iteration));
    *iter = (void *)iteration;

    iteration->dir = directory;
    iteration->lastOffset = 0;

    return VFS_PASSED;
}

inline static dword GetFileName(dword offset, dword lastOffset, void *cache, char *name) {
    // 从后往前设置文件名
    dword cnt = 0;
    offset -= sizeof(FAT32Entry);
    while (offset >= lastOffset) {
        FAT32Entry *entry = (FAT32Entry *)(cache + offset);

        bool flag = false;

        // 检测name1
        for (int i = 0 ; (i < 5) & (! flag) ; i ++) {
            if (entry->lfnEntry.name1[i] == 0) {
                flag = true;
                break;
            }
            if (name != NULL) {
                name[cnt] = entry->lfnEntry.name1[i] & 0xFF;
            }
            cnt ++;
        }

        // 检测name2
        for (int i = 0 ; (i < 6) & (! flag) ; i ++) {
            if (entry->lfnEntry.name2[i] == 0) {
                flag = true;
                break;
            }
            if (name != NULL) {
                name[cnt] = entry->lfnEntry.name2[i] & 0xFF;
            }
            cnt ++;
        }

        // 检测name3
        for (int i = 0 ; (i < 2) & (! flag) ; i ++) {
            if (entry->lfnEntry.name3[i] == 0) {
                break;
            }
            if (name != NULL) {
                name[cnt] = entry->lfnEntry.name3[i] & 0xFF;
            }
            cnt ++;
        }

        // 最终项LFN
        if ((entry->lfnEntry.order & LFN_ORDER_MASK) == LFN_ORDER_MASK) {
            break;
        }

        if (offset == lastOffset) {
            LogError("未发现最终LFN项!");
        }

        offset -= sizeof(FAT32Entry);
    }

    if (name != NULL) {
        name[cnt] = '\0';
    }

    return cnt;
}

/**
 * @brief 下个文件
 *
 * @param data FS数据
 * @param iter 迭代器
 * @param file 文件
 *
 * @return 错误号
 */
VFSErrors FAT32Next(FSData *data, void *iter, VFile *file) {
    FAT32Iteration *iteration = (FAT32Iteration *)iter;
    dword offset = iteration->lastOffset;

    // 最后一个文件
    if (offset >= iteration->dir->size) {
        return VFS_FINAL;
    }

    // 初始化文件
    InitFile(data, file);

    // 定位文件
    while (offset < iteration->dir->size) {
        FAT32Entry *entry = (FAT32Entry *)(iteration->dir->cache + offset);

        // 是有效项
        if (
            (entry->lfnEntry.order != 0) &&
            (entry->fileEntry.attr.reserved == 0)
        ) {
            // 不是.或者..文件
            if (
                (! entry->fileEntry.attr.volumnID) &&
                (memcmp(entry->fileEntry.name, ".       ", 8) != 0)&&
                (memcmp(entry->fileEntry.name, "..      ", 8) != 0)
            ) {
                // 文件项
                break;
            }
        }

        offset += sizeof(FAT32Entry);
    }

    // 最后一个文件
    if (offset >= iteration->dir->size) {
        iteration->lastOffset = offset;
        return VFS_FINAL;
    }

    FAT32Entry *entry = (FAT32Entry *)(iteration->dir->cache + offset);

    // 获取文件名
    dword len = GetFileName(offset, iteration->lastOffset, iteration->dir->cache, NULL);
    file->name = (char *)lmalloc(len + 1);
    GetFileName(offset, iteration->lastOffset, iteration->dir->cache, file->name);

    // 设置是否为目录
    file->isDirectory = entry->fileEntry.attr.directory;

    // 设置文件数据
    __FILEDATA__->entry = entry->fileEntry;

    // 设置起始簇号
    dword startClus = entry->fileEntry.startClusHigh;
    startClus <<= 16;
    startClus += entry->fileEntry.startClusLow;
    __FILEDATA__->startClus = startClus;

    if (file->isDirectory) {
        file->size = GetDirectorySize(data, __FILEDATA__->startClus);
    }
    else {
        file->size = entry->fileEntry.fileSize;
    }

    iteration->lastOffset = offset + sizeof(FAT32Entry);
    return VFS_PASSED;
}

/**
 * @brief 关闭迭代器
 *
 * @param data FS数据
 * @param iter 迭代器
 *
 * @return 错误号
 */
VFSErrors FAT32CloseIteration(FSData *data, void *iter) {
    lfree(iter);
    return VFS_PASSED;
}

/**
 * @brief 关闭文件
 *
 * @param file 文件
 *
 * @return 错误号
 */
VFSErrors FAT32Close(VFile *file) {
    lfree(file->data);
    if (file->cache != NULL) {
        lfree(file->cache);
    }
    return VFS_PASSED;
}

/**
 * @brief 读取文件
 *
 * @param file 文件
 * @param target 目标地址
 *
 * @return 错误号
 */
VFSErrors FAT32Read(VFile *file, void *target) {
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
VFSErrors FAT32Write(VFile *file, void *source, size_t size) {
    return VFS_UNIMPLEMENTED;
}


/**
 * @brief FAT32文件系统
 *
 */
FS FAT32FS = {
    .Load           = FAT32Load,
    .Unload         = FAT32Unload,
    .InitIteration  = FAT32InitIteration,
    .Next           = FAT32Next,
    .CloseIteration = FAT32CloseIteration,
    .Close          = FAT32Close,
    .Read           = FAT32Read,
    .Write          = FAT32Write
};