/**
 * @file vfs.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2022-11-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/types.h>
#include <fs/disk.h>

/**
 * @brief 文件系统
 * 
 */
typedef void fs_t;
/**
 * @brief inode
 * 
 */
typedef void inode;

/**
 * @brief VFS虚拟文件系统
 * 
 */
typedef struct _vfs_t vfs_t;

/**
 * @brief 虚拟文件系统文件
 * 
 */
typedef struct _vfile_t {
    /**
     * @brief 文件系统
     * 
     */
    vfs_t *fs;
    /**
     * @brief inode
     * 
     */
    inode *node;
    /**
     * @brief 大小
     * 
     */
    size_t size;
    /**
     * @brief 文件名
     * 
     */
    wchar *name;
    /**
     * @brief 是否为最后一个文件
     * 
     */
    bool is_last;
    /**
     * @brief 是否为目录
     * 
     */
    bool is_directory;
    /**
     * @brief 下个文件
     * 
     */
    NULLABLE struct _vfile_t *nextfile;
    /**
     * @brief 子文件
     * 
     */
    NULLABLE struct _vfile_t *childfile;
    /**
     * @brief 父文件
     * 
     */
    NONNULL struct _vfile_t *parentfile;
} vfile_t;

/**
 * @brief 读取文件
 * 
 * @param file 文件
 * @param buffer 缓存
 */
PUBLIC void read_file(NONNULL vfile_t *file, NONNULL void *buffer);
/**
 * @brief 写入文件
 * 
 * @param file 文件
 * @param buffer 缓存
 * @param size 文件大小
 */
PUBLIC void write_file(NONNULL vfile_t *file, NONNULL void *buffer, size_t size);
/**
 * @brief 释放vfile对象
 * 
 * @param file vfile对象
 */
PUBLIC void free_vfile(NONNULL vfile_t *file);
/**
 * @brief 获得下个文件
 * 
 * @param file 当前文件
 * @return 下个文件
 */
NULLABLE PUBLIC vfile_t *get_next_file(vfile_t *file);
/**
 * @brief 获得子文件
 * 
 * @param file 当前文件
 * @return 子文件
 */
NULLABLE PUBLIC vfile_t *get_child_file(vfile_t *file);

/**
 * @brief 文件系统操作
 * 每一个不同的FS都要填满这个结构体并将其指针加入到下方FSOP数组中
 */
typedef struct {
    /**
     * @brief 判断是否为对应文件系统
     * 
     */
    bool *(*check_fs)(void *pbr);
    /**
     * @brief 打开文件系统
     * 
     */
    fs_t *(*open_fs)(disk_t *disk, void *pbr);
    /**
     * @brief 释放文件系统
     * 
     */
    void (*free_fs)(fs_t *fs);
    /**
     * @brief 获得根目录
     * 
     */
    vfile_t *(*get_root)(fs_t *fs);
    /**
     * @brief 获得下一个文件
     * 
     */
    vfile_t *(*get_next_file)(fs_t *fs, vfile_t *file);
    /**
     * @brief 获得子文件
     * 
     */
    vfile_t *(*get_child_file)(fs_t *fs, vfile_t *file);
    /**
     * @brief 读文件
     * 
     */
    void (*read_file)(fs_t *fs, vfile_t *file, void *buffer);
    /**
     * @brief 读文件
     * 
     */
    void (*write_file)(fs_t *fs, vfile_t *file, void *buffer, size_t size);
} fs_operation;

/**
 * @brief 文件系统操作组数
 * 
 */
#define FSOPGROUP_NUM (16)

/**
 * @brief 文件系统操作组数组
 * 
 */
EXTERN fs_operation *FSOPGROUPS[FSOPGROUP_NUM];