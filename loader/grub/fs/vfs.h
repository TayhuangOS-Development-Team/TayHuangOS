/**
 * @file vfs.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <device/disk.h>
#include <stddef.h>

struct FSDataStruct;
struct FSStruct;

/**
 * @brief VFS文件
 *
 */
typedef struct VFileStruct {
    /** 外部数据 */
    /** 文件所在硬盘 */
    Disk *disk;
    /** 文件所在分区 */
    Partition *part;
    /** 文件所在文件系统 */
    struct FSDataStruct *fs;

    /** 文件专用数据 */
    void *data;
    /** 文件缓存 */
    void *cache;
    /** 文件大小 */
    size_t size;
    /** 文件名*/
    char *name;
    /** 是否为目录 */
    bool isDirectory;

    /** 上一个文件 */
    struct VFileStruct *last;
    /** 下一个文件 */
    struct VFileStruct *next;
} VFile;

typedef enum {
    VFS_PASSED = 0,
    VFS_ERROR = 1,
    VFS_FATAL = 2,
    VFS_INVALID = 3,
    VFS_LOAD_FAILED = 4,
    VFS_READ_FAILED = 5,
    VFS_WRITE_FAILED = 6,
    VFS_NOT_EXIST = 7,
    VFS_UNIMPLEMENTED = 8
} VFSErrors;

/**
 * @brief 文件系统数据
 *
 */
typedef struct FSDataStruct {
    /** 外部数据 */
    /** 文件系统 */
    struct FSStruct *fs;
    /** 文件系统所在硬盘 */
    Disk *disk;
    /** 文件系统所在分区 */
    Partition *part;

    /** 文件系统专用数据 */
    void *data;
    /** 根目录 */
    VFile *root;
} FSData;

/**
 * @brief 文件系统
 *
 */
typedef struct FSStruct {
    /** 上个文件系统 */
    struct FSStruct *last;
    /** 下个文件系统 */
    struct FSStruct *next;
    /** 文件系统名称 */
    const char *name;
    /**
     * @brief 加载文件系统
     *
     * @param part 分区
     * @param data FS数据(在Load外自行Malloc)
     *
     * @return 错误号
     */
    VFSErrors (*Load)(Partition *part, FSData *data);
    /**
     * @brief 卸载文件系统
     *
     * @param data FS数据(在Unload外自行Free)
     *
     * @return 错误号
     */
    VFSErrors (*Unload)(FSData *data);
    /**
     * @brief 遍历文件
     *
     * @param data FS数据
     * @param directory 目录文件
     * @param head 首个文件
     *
     * @return 错误号
     */
    VFSErrors (*Foreach)(FSData *data, VFile *directory, VFile *head);
    /**
     * @brief 关闭文件
     *
     * @param file 文件
     *
     * @return 错误号
     */
    VFSErrors (*Close)(VFile *file);
    /**
     * @brief 读取文件
     *
     * @param file 文件
     * @param target 目标地址
     *
     * @return 错误号
     */
    VFSErrors (*Read)(VFile *file, void *target);
    /**
     * @brief 写文件
     *
     * @param file 文件
     * @param source 源地址
     *
     * @return 错误号
     */
    VFSErrors (*Write)(VFile *file, void *source, size_t size);
} FS;

/**
 * @brief 注册文件系统
 *
 * @param fs 文件系统
 */
void RegisterFS(FS *fs);

/**
 * @brief 加载文件系统
 *
 * @param part 分区
 * @return 文件系统
 */
FSData *LoadFS(Partition *part);

/**
 * @brief 卸载文件系统
 *
 * @param data 文件系统
 */
void UnloadFS(FSData *data);