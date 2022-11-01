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
 * @brief FAT16 文件系统魔数
 * 
 */
#define FS_FAT16       (0xE0993414)
/**
 * @brief FAT32 文件系统魔数
 * 
 */
#define FS_FAT32       (0x12007674)
/**
 * @brief NTFS 文件系统魔数
 * 
 */
#define FS_NTFS        (0x2ED7B522)
/**
 * @brief EXT2 文件系统魔数
 * 
 */
#define FS_EXT2        (0x0F74975E)
/**
 * @brief EXT3 文件系统魔数
 * 
 */
#define FS_EXT3        (0x5F941BBF)
/**
 * @brief EXT4 文件系统魔数
 * 
 */
#define FS_EXT4        (0xB1B3A020)
/**
 * @brief 未知文件系统魔数
 * 
 */
#define FS_UNKNOWN     (0x254C4732)

/**
 * @brief 文件系统
 * 
 */
typedef void fs_t;
/**
 * @brief bnode - block node
 * 块节点
 * 
 */
typedef void bnode;
/**
 * @brief fnode - file node
 * 文件节点
 * 
 */
typedef void fnode;
/**
 * @brief dnode - directory node
 * 目录节点
 * 
 */
typedef void dnode;
typedef struct {
    dnode *dir;
    bool is_last;
    bool is_leaf;
    bool is_root;
    char *name;
    fnode *file;
    size_t filesz;
} dnode_result;

typedef struct {
    bnode *block;
    size_t size;
    bool is_last;
} bnode_result;

/**
 * @brief 文件系统函数表
 * 其指针应位于特定文件系统结构体的开头
 * 
 */
typedef struct {
    void (*close_fs)(fs_t *fs);
    dnode_result (*get_root)(fs_t *fs);
    dnode_result (*get_next_dnode)(fs_t *fs, dnode *dir);
    dnode_result (*get_child_dnode)(fs_t *fs, dnode *dir);
    bnode_result (*get_first_block)(fs_t *fs, fnode *file);
    bnode_result (*get_next_block)(fs_t *fs, bnode *block);
    void (*get_block_data)(fs_t *fs, bnode *block, void *buffer);
} fs_func_table;

typedef struct _vfs_t vfs_t;

struct _vbnode {
    bnode *block;
    vfs_t *fs;
    struct _vbnode *next;
    bool is_last;
    size_t size;
};
typedef struct _vbnode vbnode;

struct _vfnode {
    fnode *file;
    vfs_t *fs;
    vbnode *first_block;
    char *name;
    size_t size;
};
typedef struct _vfnode vfnode;

struct _vdnode {
    dnode *dir;
    vfs_t *fs;
    vfnode *file;
    struct _vdnode *next;
    struct _vdnode *child;
    struct _vdnode *parent;
    bool is_last;
    bool is_leaf;
    bool is_root;
    char *name;
};
typedef struct _vdnode vdnode;
struct _vfs_t {
    fs_t *fs;
    vdnode *root_dir;
    const char *fs_type;
    size_t fs_size;
};

/**
 * @brief 打开文件系统
 * 
 * @param disk 磁盘
 * @return 文件系统对象
 */
PUBLIC vfs_t *open_fs(disk_t *disk);
/**
 * @brief 关闭文件系统
 * 
 * @param fs 文件系统
 */
PUBLIC void close_fs(vfs_t *fs);
/**
 * @brief 获得文件系统大小
 * 
 * @param fs 文件系统
 * @return 文件系统大小 
 */
PUBLIC size_t get_fs_size(vfs_t *fs);
/**
 * @brief 获得文件系统类型
 * 
 * @param fs 文件系统
 * @return 文件系统类型
 */
PUBLIC const char *get_fs_type(vfs_t *fs);

/**
 * @brief 获得根目录
 * 
 * @param fs 文件系统
 * @return 根目录
 */
PUBLIC vdnode *get_root_dir(vfs_t *fs);
/**
 * @brief 获得目录节点对应的文件节点
 * 
 * @param dir 目录节点
 * @return 文件节点
 */
PUBLIC vfnode *get_dir_file(vdnode *dir);
/**
 * @brief 获得该层的下一个目录节点
 * 
 * @param dir 目录节点
 * @return 下一个目录节点
 */
PUBLIC vdnode *get_next_dnode(vdnode *dir);
/**
 * @brief 获得该目录节点的起始子节点
 * 
 * @param dir 目录节点
 * @return 起始子节点
 */
PUBLIC vdnode *get_child_dnode(vdnode *dir);
/**
 * @brief
 * 
 * @param dir 
 * @return 
 */
PUBLIC vdnode *get_parent_dnode(vdnode *dir);

/**
 * @brief 获得文件起始块节点
 * 
 * @param file 文件节点
 * @return 起始块节点
 */
PUBLIC vbnode *get_first_block(vfnode *file);
/**
 * @brief 获得文件数据
 * 
 * @param file 文件节点
 * @param buffer 缓存
 */
PUBLIC void get_file_data(vfnode *file, void *buffer);

/**
 * @brief 获得块数据
 * 
 * @param block 块节点
 * @param buffer 缓存
 */
PUBLIC void get_block_data(vbnode *block, void *buffer);
/**
 * @brief 获得下个块节点
 * 
 * @param block 块节点
 * @return 下个块节点
 */
PUBLIC vbnode *get_next_block(vbnode *block);