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
#include <fs/fs/fat32.h>

#include <heap.h>
#include <logging.h>

#include <string.h>

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
 * @brief 获得文件系统函数表
 * 
 * @param fs 文件系统
 * @return 文件系统函数表
 */
INLINE fs_func_table *fs_funcs(fs_t *fs) {
    return *(fs_func_table **)(fs);
}

typedef struct {
    fs_t *fs;
    const char *type;
    size_t size;
} fs_result;

/**
 * @brief 
 * 
 * @param pbr 分区引导记录
 * @return
 */
PRIVATE fs_result __open_fs(void *pbr) {
    if (is_fat32_fs(pbr)) {
        return (fs_result) {
            .fs = open_fat32_fs(pbr), 
            .type = "FAT32",
            .size = get_fat32_fs_size()
        };
    }
    return (fs_result) {
        .fs = NULL,
        .type = "Unknown",
        .size = 0
    };
}

PUBLIC vfs_t *open_fs(disk_t *disk) {
    char _pbr[512];
    void *pbr = _pbr;
    read_sector(disk, 0, 1, pbr);
    vfs_t *vfs = malloc(sizeof(vfs));
    fs_result result = __open_fs(pbr);
    vfs->fs = result.fs;
    vfs->fs_type = result.type; 
    vfs->fs_size = sizeof(vfs_t) + result.size;
    return vfs;
}

PUBLIC void close_fs(vfs_t *fs) {
    if (fs->fs != NULL) {
        fs_funcs(fs->fs)->close_fs(fs);
    }
    free(fs);
}

PUBLIC size_t get_fs_size(vfs_t *fs) {
    return fs->fs_size;
}

PUBLIC const char *get_fs_type(vfs_t *fs) {
    return fs->fs_type;
}

INLINE vfnode *packvf(vfs_t *fs, fnode *file, size_t size, char *name) {
    vfnode *vf = malloc(sizeof(vfnode));
    vf->file = file;
    vf->size = size;
    vf->name = name;
    vf->fs = fs;
    return vf;
}

INLINE vdnode *packvd(vfs_t *fs, dnode *dir, bool is_last, bool is_leaf, bool is_root,
                     char *name, size_t size, fnode *file) {
    vdnode *vd = malloc(sizeof(vdnode));
    vd->dir = dir;
    vd->next = vd->parent = vd->child = NULL;
    vd->file = packvf(fs, file, size, name);
    vd->is_last = is_last;
    vd->is_leaf = is_leaf;
    vd->is_root = is_root;
    vd->name = name;
    vd->fs = fs;
    return vd;
}

INLINE vbnode *packvb(vfs_t *fs, bnode *block, size_t size, bool is_last) {
    vbnode *vb = malloc(sizeof(vbnode));
    vb->block = block;
    vb->fs = fs;
    vb->is_last = is_last;
    vb->next = NULL;
    vb->size = size;
    return vb;
}

PUBLIC vdnode *get_root_dir(vfs_t *fs) {
    if (fs->root_dir == NULL) {
        dnode_result root = fs_funcs(fs->fs)->get_root(fs->fs);
        fs->root_dir = packvd(fs, root.dir, root.is_last, root.is_leaf, root.is_root,
                         root.name, root.filesz, root.file);
    }
    return fs->root_dir;
}

PUBLIC vfnode *get_dir_file(vdnode *dir) {
    if (dir->file == NULL) {
        LERROR ("GRUB2 Loader VFS", "NULL file of directory");
    }
    return dir->file;
}

PUBLIC vdnode *get_next_dnode(vdnode *dir) {
    if (dir->is_last) {
        return NULL;
    }
    if (dir->next == NULL) {
        dnode_result result = fs_funcs(dir->fs->fs)->get_next_dnode(dir->fs->fs, dir->dir);
        dir->next = packvd(dir->fs, result.dir, result.is_last, result.is_leaf, result.is_root,
            result.name, result.filesz, result.file);
    }
    return dir->next;
}

PUBLIC vdnode *get_child_dnode(vdnode *dir) {
    if (dir->is_leaf) {
        return NULL;
    }
    if (dir->child == NULL) {
        dnode_result result = fs_funcs(dir->fs->fs)->get_child_dnode(dir->fs->fs, dir->dir);
        dir->next = packvd(dir->fs, result.dir, result.is_last, result.is_leaf, result.is_root,
            result.name, result.filesz, result.file);
    }
    return dir->child;
}


PUBLIC vbnode *get_first_block(vfnode *file) {
    if (file->first_block == NULL) {
        bnode_result result = fs_funcs(file->fs->fs)->get_first_block(file->fs->fs, file->file);
        file->first_block = packvb(file->fs, result.block, result.size, result.is_last);
    }
    return file->first_block;
}

PUBLIC void get_file_data(vfnode *file, void *buffer) {
    vbnode *block = get_first_block(file);
    size_t remain_sz = file->size;

    void *buf = malloc(block->size);
    size_t buf_sz = block->size;

    size_t offset = 0;
    while (! (block->is_last && remain_sz > 0)) {
        if (block->size > buf_sz) {
            free (buf);
            buf = malloc(block->size);
            buf_sz = block->size;
        }
        
        get_block_data(block, buf);
        size_t cp_sz = remain_sz > buf_sz ? buf_sz : remain_sz;
        memcpy(buffer + offset, buf, cp_sz);
        offset += cp_sz;
        remain_sz -= cp_sz;
    }
}


PUBLIC void get_block_data(vbnode *block, void *buffer) {
    fs_funcs(block->fs->fs)->get_block_data(block->fs->fs, block->block, buffer);
}

PUBLIC vbnode *get_next_block(vbnode *block) {
    if (block->is_last) {
        return NULL;
    }
    if (block->next == NULL) {
        bnode_result result = fs_funcs(block->fs->fs)->get_next_block(block->fs->fs, block->block);
        block->next = packvb(block->fs, result.block, result.size, result.is_last);
    }
    return block->next;
}