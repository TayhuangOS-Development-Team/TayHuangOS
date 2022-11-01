/**
 * @file fat32.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/fs/fat32.h>
#include <string.h>
#include <logging.h>
#include <heap.h>

#define BPB_FILESYSTEM (0x52)

/**
 * @brief fat32文件系统操作表
 * 
 */
PRIVATE struct fat32op_t {
    void (*close_fs)(fs_t *fs);
    dnode_result (*get_root)(fs_t *fs);
    dnode_result (*get_next_dnode)(fs_t *fs, dnode *dir);
    dnode_result (*get_child_dnode)(fs_t *fs, dnode *dir);
    bnode_result (*get_first_block)(fs_t *fs, fnode *file);
    bnode_result (*get_next_block)(fs_t *fs, bnode *block);
    void (*get_block_data)(fs_t *fs, bnode *block, void *buffer);
} fat32op = {
    .close_fs = close_fat32_fs
};

/**
 * @brief fat32块节点
 * 
 */
typedef struct {
} fat32bnode;

/**
 * @brief fat32文件节点
 * 
 */
typedef struct {
} fat32fnode;

/**
 * @brief fat32目录节点
 * 
 */
typedef struct {
} fat32dnode;

/**
 * @brief fat32
 * 
 */
typedef struct {
    struct fat32op_t *op_tab;
} fat32fs_t;

PUBLIC bool is_fat32_fs(void *pbr) {
    char fs_type[9];
    //获取BPB中FS号
    memcpy(fs_type, pbr + BPB_FILESYSTEM, 8);
    fs_type[8] = '\0';
    return strcmp(fs_type, "FAT32   ") == 0;
}

/**
 * @brief 打开fat32文件系统
 * 
 * @param pbr 分区引导记录
 * @return fat32文件系统
 */
INLINE fat32fs_t *__open_fat32_fs__(void *pbr) {
    fat32fs_t *fs = malloc(sizeof(fat32fs_t));
    fs->op_tab = &fat32op;
    return fs;
}

/**
 * @brief 关闭fat32文件系统
 * 
 * @param fs fat32文件系统
 */
INLINE void __close_fat32_fs__(fat32fs_t *fs) {
    free(fs);
}

PUBLIC fs_t *open_fat32_fs(void *pbr) {
    return (fs_t *)(__open_fat32_fs__(pbr));
}

PUBLIC void close_fat32_fs(fs_t *fs) {
    __close_fat32_fs__((fat32fs_t *)fs);
}