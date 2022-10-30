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

/**
 * @brief 获得文件系统魔数
 * 
 * @param fs 文件系统
 * @return 文件系统魔数 
 */
INLINE dword get_fs_magic(fs_t *fs) {
    return *(dword *)fs;
}

/**
 * @brief 获得文件系统类型
 * 
 * @param pbr 分区引导记录
 * @return 文件系统魔数
 */
INLINE dword get_disk_fs_type(void *pbr) {
    return FS_UNKNOWN;
}

PUBLIC fs_t *open_fs(disk_t *disk) {
    void *pbr = malloc(512);
    read_sector(disk, 0, 1, pbr);
    fs_t *fs;
    dword fs_type = get_disk_fs_type(pbr);
    if (fs_type == FS_FAT32) {
        //...
    }
    else if (fs_type == FS_UNKNOWN) {
        return NULL;
    }
    free(pbr);
    return fs;
}

PUBLIC void close_fs(fs_t *fs) {
    dword fs_type = get_fs_magic(fs);
    if (fs_type == FS_FAT32) {
        //...
    }
    else if (fs_type == FS_UNKNOWN) {
        LERROR ("GRUB2 Loader VFS", "Unknown type of FS");
        return;
    }
    LERROR ("GRUB2 Loader VFS", "Unsupported FS");
    return;
}

PUBLIC size_t get_fs_size(fs_t *fs) {
    dword fs_type = get_fs_magic(fs);
    if (fs_type == FS_FAT32) {
        //...
    }
    else if (fs_type == FS_UNKNOWN) {
        LERROR ("GRUB2 Loader VFS", "Unknown type of FS");
        return 0;
    }
    LERROR ("GRUB2 Loader VFS", "Unsupported FS");
    return 0;
}

PUBLIC const char *get_fs_type(fs_t *fs) {
    dword fs_type = get_fs_magic(fs);
    if (fs_type == FS_FAT32) {
        //...
    }
    else if (fs_type == FS_UNKNOWN) {
        LERROR ("GRUB2 Loader VFS", "Unknown type of FS");
        return "Unknown";
    }
    LERROR ("GRUB2 Loader VFS", "Unsupported FS");
    return "Unsupported";
}


PUBLIC dnode *get_root_dir(fs_t *fs);

PUBLIC bool is_last_dnode(fs_t *fs, dnode *dir);

PUBLIC bool is_leaf_dnode(fs_t *fs, dnode *dir);

PUBLIC fnode *get_dir_file(fs_t *fs, dnode *dir);

PUBLIC dnode *get_next_dnode(fs_t *fs, dnode *dir);

PUBLIC dnode *get_child_dnode(fs_t *fs, dnode *dir);

PUBLIC const char *get_dnode_name(fs_t *fs, dnode *dir);


PUBLIC const char *get_file_name(fs_t *fs, fnode *file);

PUBLIC bnode *get_first_block(fs_t *fs, fnode *file);

PUBLIC size_t get_file_size(fs_t *fs, fnode *file);

PUBLIC void get_file_data(fs_t *fs, fnode *file, void *buffer);


PUBLIC size_t get_block_size(fs_t *fs, bnode *block);

PUBLIC void get_block_data(fs_t *fs, bnode *block, void *buffer);

PUBLIC bool is_last_bnode(fs_t *fs, bnode *block);

PUBLIC bnode *get_next_block(fs_t *fs, bnode *block);