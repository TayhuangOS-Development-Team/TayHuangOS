/**
 * @file vfs.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2023-08-09
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/vfs.h>
#include <libs/capi.h>

/** 文件系统列表 */
static FS *fsList = NULL;

/**
 * @brief 注册文件系统
 * 
 * @param fs 文件系统
 */
void RegisterFS(FS *fs) {
    // 表为空
    if (fsList == NULL) {
        fsList = fs;
        fs->last = fs->next = NULL;
        return;
    }

    // 遍历到表尾
    FS *last = fsList;
    while (last->next != NULL) {
        last = last->next;
    }

    // 加入链表
    last->next = fs;
    fs->last = last;
    fs->next = NULL;
}

/**
 * @brief 加载文件系统
 * 
 * @param part 分区
 * @return 文件系统
 */
FSData *LoadFS(Partition *part) {
    // 表为空
    if (fsList == NULL) {
        return NULL;
    }

    FSData *data = (FSData *)lmalloc(sizeof(FSData));

    FS *fs = fsList;
    while (fs != NULL) {
        // 若加载成功
        if (fs->Load(part, data) == VFS_PASSED) {
            return data;
        }
        fs = fs->next;
    }

    // 无法加载
    lfree(data);
    return NULL;
}

/**
 * @brief 卸载文件系统
 * 
 * @param data 文件系统
 */
void UnloadFS(FSData *data) {
    data->fs->Unload(data);
    lfree(data);
}