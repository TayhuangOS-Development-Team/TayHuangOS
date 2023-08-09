/**
 * @file fat32.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32文件系统
 * @version alpha-1.0.0
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <fs/vfs.h>

/**
 * @brief 加载文件系统
 *
 * @param part 分区
 * @param data FS数据(在Load外自行Malloc)
 *
 * @return 错误号
 */
VFSErrors Fat32Load(Partition *part, FSData *data);
/**
 * @brief 卸载文件系统
 *
 * @param data FS数据(在Unload外自行Free)
 *
 * @return 错误号
 */
VFSErrors Fat32Unload(FSData *data);
/**
 * @brief 遍历文件
 *
 * @param data FS数据
 * @param directory 目录文件
 * @param head 首个文件
 *
 * @return 错误号
 */
VFSErrors Fat32Foreach(FSData *data, VFile *directory, VFile *head);
/**
 * @brief 关闭文件
 *
 * @param file 文件
 *
 * @return 错误号
 */
VFSErrors Fat32Close(VFile *file);
/**
 * @brief 读取文件
 *
 * @param file 文件
 * @param target 目标地址
 *
 * @return 错误号
 */
VFSErrors Fat32Read(VFile *file, void *target);
/**
 * @brief 写文件
 *
 * @param file 文件
 * @param source 源地址
 *
 * @return 错误号
 */
VFSErrors Fat32Write(VFile *file, void *source, size_t size);

/**
 * @brief FAT32文件系统
 *
 */
extern FS FAT32FS;