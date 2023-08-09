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