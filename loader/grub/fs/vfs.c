/**
 * @file vfs.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/vfs.h>

#include <heap.h>
#include <logging.h>

#include <string.h>

/**
 * @brief 操作数
 * 
 */
#define NUM_SOP (16)
/**
 * @brief 操作列表
 * 
 */
PRIVATE superop_t *OPERATORS[NUM_SOP] = {};

/**
 * @brief 查找sop
 * 
 * @param pbr PBR
 * @return sop
 */
PRIVATE superop_t *lookup_sop(void *pbr) {
    for (int i = 0 ; i < NUM_SOP ; i ++) {
        // 非NULL
        if (OPERATORS[i] == NULL) {
            continue;
        }
        // 是对应FS
        if (OPERATORS[i]->check_fs(pbr)) {
            return OPERATORS[i];
        }
    }
    return NULL;
}

PUBLIC CONSTRUCTOR vfs_t *open_fs(disk_t *disk) {
    // 获取PBR
    void *pbr = malloc(512);
    read_sector(disk, 0, 1, pbr);
    
    // 获取sop
    superop_t *sop = lookup_sop(pbr);
    if (sop == NULL) {
        // 未知FS
        LERROR ("GRUB2 Loader", "Unsupported FS");
        return NULL;
    }

    // 获取VFS指针
    vfs_t *vfs = sop->open_fs(disk, pbr);
    free(pbr);
    return vfs;
}
/**
 * @brief 获得子文件
 * 
 * @param parent 父文件
 * @return 子文件
 */
PUBLIC inode_t *get_child(THIS inode_t *parent) {
    vfs_t *fs = parent->fs;
    inode_t *child = fs->sop->load_child(fs, parent);
    return child;
}
/**
 * @brief 获得文件数据
 * 
 * @param file 文件
 * @param buffer 缓存
 * @param size 缓存大小
 */
PUBLIC void readfile(THIS inode_t *file, void *buffer, size_t size) {
    vfs_t *fs = file->fs;
    void *buf = fs->sop->readfile(fs, file);
    size_t cpysize = file->filesize > size ? size : file->filesize;
    memcpy(buffer, buf, cpysize);
}
/**
 * @brief 关闭文件系统
 * 
 * @param fs 文件系统
 */
PUBLIC DECONSTRUCTOR void close_fs(vfs_t *fs);