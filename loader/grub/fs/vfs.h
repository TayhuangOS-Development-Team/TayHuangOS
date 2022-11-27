/**
 * @file vfs.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <fs/disk.h>

/**
 * @brief inode额外信息
 * 
 */
typedef void iext_t;
/**
 * @brief fs额外信息
 * 
 */
typedef void fsext_t;

/**
 * @brief VFS
 * 
 */
struct _vfs_t;
/**
 * @brief VFS
 * 
 */
typedef struct _vfs_t vfs_t;

/**
 * @brief inode
 * 
 */
typedef struct _inode_t {
    /**
     * @brief 文件名
     * 
     */
    wchar *name;
    /**
     * @brief 兄弟文件(为NULL说明是最后一个)
     * 
     */
    struct _inode_t *next;
    /**
     * @brief 父文件(为NULL说明是根目录)
     * 
     */
    struct _inode_t *parent;
    /**
     * @brief 子文件
     * 
     */
    NULLABLE struct _inode_t *child;
    /**
     * @brief 是否为目录
     * 
     */
    bool is_directory;
    /**
     * @brief 文件大小
     * 
     */
    size_t filesize;
    /**
     * @brief inode附加信息
     * 
     */
    iext_t *inode;
    /**
     * @brief 文件系统
     * 
     */
    vfs_t *fs;
} inode_t;

typedef struct {
    /**
     * @brief 检测文件系统
     * 
     */
    bool (*check_fs)(void *pbr);
    /**
     * @brief 打开文件系统
     * 
     */
    CONSTRUCTOR vfs_t *(*open_fs)(disk_t *disk, void *pbr);
    /**
     * @brief 获得根目录
     * 
     */
    inode_t *(*get_root)(THIS vfs_t *fs);
    /**
     * @brief 加载子文件
     * 
     */
    inode_t *(*load_child)(THIS vfs_t *fs, inode_t *last);
    /**
     * @brief 获得文件数据(只读)
     * 
     */
    void *(*readfile)(THIS vfs_t *fs, inode_t *file);
    /**
     * @brief 清除拓展信息
     * 
     */
    DECONSTRUCTOR void (*terminate_iext)(THIS inode_t *inode);
    /**
     * @brief 清除拓展信息
     * 
     */
    DECONSTRUCTOR void (*terminate_fsext)(THIS vfs_t *fs);
} superop_t;

/**
 * @brief VFS
 * 
 */
struct _vfs_t {
    /**
     * @brief 根目录
     * 
     */
    NONNULL inode_t *root;
    /**
     * @brief 文件系统类型
     * 
     */
    const char *fs_type;
    /**
     * @brief 硬盘
     * 
     */
    disk_t *disk;
    /**
     * @brief 文件系统
     * 
     */
    fsext_t *fs;
    /**
     * @brief 文件系统超级操作
     * 
     */
    superop_t *sop;
};

/**
 * @brief 打开文件系统
 * 
 * @param disk 硬盘
 * @return 文件系统
 */
PUBLIC CONSTRUCTOR vfs_t *open_fs(disk_t *disk);
/**
 * @brief 获得子文件
 * 
 * @param parent 父文件
 * @return 子文件
 */
PUBLIC inode_t *get_child(THIS inode_t *parent);
/**
 * @brief 获得文件数据
 * 
 * @param file 文件
 * @param buffer 缓存
 * @param size 缓存大小
 */
PUBLIC void readfile(THIS inode_t *file, void *buffer, size_t size);
/**
 * @brief 关闭文件系统
 * 
 * @param fs 文件系统
 */
PUBLIC DECONSTRUCTOR void close_fs(vfs_t *fs);