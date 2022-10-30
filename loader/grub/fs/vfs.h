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
 * @brief 打开文件系统
 * 
 * @param disk 磁盘
 * @return 文件系统对象
 */
PUBLIC fs_t *open_fs(disk_t *disk);
/**
 * @brief 关闭文件系统
 * 
 * @param fs 文件系统
 */
PUBLIC void close_fs(fs_t *fs);
/**
 * @brief 获得文件系统大小
 * 
 * @param fs 文件系统
 * @return 文件系统大小 
 */
PUBLIC size_t get_fs_size(fs_t *fs);
/**
 * @brief 获得文件系统类型
 * 
 * @param fs 文件系统
 * @return 文件系统类型
 */
PUBLIC const char *get_fs_type(fs_t *fs);

/**
 * @brief dnode - directory node
 * 目录节点
 * 
 */
typedef void dnode;
/**
 * @brief fnode - file node
 * 文件节点
 * 
 */
typedef void fnode;
/**
 * @brief bnode - block node
 * 块节点
 * 
 */
typedef void bnode;

/**
 * @brief 获得根目录
 * 
 * @param fs 文件系统
 * @return 根目录
 */
PUBLIC dnode *get_root_dir(fs_t *fs);
/**
 * @brief 是否为该层中最后的目录节点
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return true 是该层中最后的目录节点
 * @return false 不是该层中最后的目录节点
 */
PUBLIC bool is_last_dnode(fs_t *fs, dnode *dir);
/**
 * @brief 是否为叶子目录节点(即文件)
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return true 是该层中叶子目录节点(即文件)
 * @return false 不是该层中叶子目录节点(即目录)
 */
PUBLIC bool is_leaf_dnode(fs_t *fs, dnode *dir);
/**
 * @brief 获得目录节点对应的文件节点
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return 文件节点
 */
PUBLIC fnode *get_dir_file(fs_t *fs, dnode *dir);
/**
 * @brief 获得该层的下一个目录节点
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return 下一个目录节点
 */
PUBLIC dnode *get_next_dnode(fs_t *fs, dnode *dir);
/**
 * @brief 获得该目录节点的起始子节点
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return 起始子节点
 */
PUBLIC dnode *get_child_dnode(fs_t *fs, dnode *dir);
/**
 * @brief 获得目录节点名
 * 
 * @param fs 文件系统
 * @param dir 目录节点
 * @return 目录节点名
 */
PUBLIC const char *get_dnode_name(fs_t *fs, dnode *dir);

/**
 * @brief 获得文件名
 * 
 * @param fs 文件系统
 * @param file 文件节点
 * @return 文件名
 */
PUBLIC const char *get_file_name(fs_t *fs, fnode *file);
/**
 * @brief 获得文件起始块节点
 * 
 * @param fs 文件系统
 * @param file 文件节点
 * @return 起始块节点
 */
PUBLIC bnode *get_first_block(fs_t *fs, fnode *file);
/**
 * @brief 获得文件大小
 * 
 * @param fs 文件系统
 * @param file 文件节点
 * @return 文件大小
 */
PUBLIC size_t get_file_size(fs_t *fs, fnode *file);
/**
 * @brief 获得文件数据
 * 
 * @param fs 文件系统
 * @param file 文件节点
 * @param buffer 缓存
 */
PUBLIC void get_file_data(fs_t *fs, fnode *file, void *buffer);

/**
 * @brief 获得块大小
 * 
 * @param fs 文件系统
 * @param block 块节点
 * @return 块大小
 */
PUBLIC size_t get_block_size(fs_t *fs, bnode *block);
/**
 * @brief 获得块数据
 * 
 * @param fs 文件系统
 * @param block 块节点
 * @param buffer 缓存
 */
PUBLIC void get_block_data(fs_t *fs, bnode *block, void *buffer);
/**
 * @brief 是否为最后一个块节点
 * 
 * @param fs 文件系统
 * @param block 块节点
 * @return true 是最后一个块节点
 * @return false 不是最后一个块节点
 */
PUBLIC bool is_last_bnode(fs_t *fs, bnode *block);
/**
 * @brief 获得下个块节点
 * 
 * @param fs 文件系统
 * @param block 块节点
 * @return下个块节点
 */
PUBLIC bnode *get_next_block(fs_t *fs, bnode *block);