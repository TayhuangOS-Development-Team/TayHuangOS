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
PUBLIC fs_t *open_fs(disk_t disk);
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
 * @param dir 目录节点
 * @return true 是该层中最后的目录节点
 * @return false 不是该层中最后的目录节点
 */
PUBLIC bool is_last_dnode(dnode *dir);
/**
 * @brief 是否为叶子目录节点(即文件)
 * 
 * @param dir 目录节点
 * @return true 是该层中叶子目录节点(即文件)
 * @return false 不是该层中叶子目录节点(即目录)
 */
PUBLIC bool is_leaf_dnode(dnode *dir);
/**
 * @brief 获得目录节点对应的文件节点
 * 
 * @param dir 目录节点
 * @return 文件节点
 */
PUBLIC fnode *get_dir_file(dnode *dir);
/**
 * @brief 获得该层的下一个目录节点
 * 
 * @param dir 目录节点
 * @return 下一个目录节点
 */
PUBLIC dnode *get_next_dnode(dnode *dir);
/**
 * @brief 获得该目录节点的起始子节点
 * 
 * @param dir 目录节点
 * @return 起始子节点
 */
PUBLIC dnode *get_child_dnode(dnode *dir);
/**
 * @brief 获得目录节点名
 * 
 * @param dir 目录节点
 * @return 目录节点名
 */
PUBLIC const char *get_dnode_name(dnode *dir);

/**
 * @brief 获得文件名
 * 
 * @param file 文件节点
 * @return 文件名
 */
PUBLIC const char *get_file_name(fnode *file);
/**
 * @brief 获得文件起始块节点
 * 
 * @param file 文件节点
 * @return 起始块节点
 */
PUBLIC bnode *get_first_block(fnode *file);
/**
 * @brief 获得文件大小
 * 
 * @param file 文件节点
 * @return 文件大小
 */
PUBLIC size_t get_file_size(fnode *file);
/**
 * @brief 获得文件数据
 * 
 * @param file 文件节点
 * @param buffer 缓存
 */
PUBLIC void get_file_data(fnode *file, void *buffer);

/**
 * @brief 获得块大小
 * 
 * @param block 块节点
 * @return 块大小
 */
PUBLIC size_t get_block_size(bnode *block);
/**
 * @brief 获得块数据
 * 
 * @param block 块节点
 * @param buffer 缓存
 */
PUBLIC void get_block_data(bnode *block, void *buffer);
/**
 * @brief 是否为最后一个块节点
 * 
 * @param block 块节点
 * @return true 是最后一个块节点
 * @return false 不是最后一个块节点
 */
PUBLIC bool is_last_bnode(bnode *block);
/**
 * @brief 获得下个块节点
 * 
 * @param block 块节点
 * @return下个块节点
 */
PUBLIC bnode *get_next_block(bnode *block);