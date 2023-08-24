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
#include <string.h>
#include <basec/logger.h>

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
        fs->lastFS = fs->nextFS = NULL;
        return;
    }

    // 遍历到表尾
    FS *lastFS = fsList;
    while (lastFS->nextFS != NULL) {
        lastFS = lastFS->nextFS;
    }

    // 加入链表
    lastFS->nextFS = fs;
    fs->lastFS = lastFS;
    fs->nextFS = NULL;
}

/**
 * @brief 加载文件系统
 *
 * @param partition 分区
 * @return 文件系统
 */
FSData *LoadFS(Partition *partition) {
    // 表为空
    if (fsList == NULL) {
        return NULL;
    }

    FSData *data = (FSData *)lmalloc(sizeof(FSData));

    data->diskPtr = partition->diskPtr;
    data->partitionPtr = partition;

    FS *fs = fsList;
    while (fs != NULL) {
        // 若加载成功
        if (fs->Load(partition, data) == VFS_PASSED) {
            data->fsFunctionTable = fs;
            return data;
        }
        fs = fs->nextFS;
    }

    // 无法加载
    lfree(data);
    return NULL;
}

/**
 * @brief 卸载文件系统
 *
 * @param fs 文件系统
 */
void UnloadFS(FSData *fs) {
    fs->fsFunctionTable->Unload(fs);
    lfree(fs);
}

/**
 * @brief 打开文件
 *
 * @param fs 文件系统
 * @param path 路径
 * @return 文件
 */
VFile *OpenFile(FSData *fs, const char *path) {
    const char *backupPathPtr = path;

    VFile *dir = fs->rootDirectory;
    char filename[512];
    while (*path != '\0') {
        // 去除开头的/
        if (*path == '/') path ++;

        // 获取文件名
        int cnt = 0;
        while (path[cnt] != '\0' && path[cnt] != '/') {
            cnt ++;
        }

        memcpy(filename, path, cnt);
        filename[cnt] = '\0';

        path += cnt;

        // 遍历寻找文件
        void *iter;

        if (fs->fsFunctionTable->InitIteration(fs, dir, &iter) != VFS_PASSED) {
            LogError("未能初始化迭代器!");
            return NULL;
        }

        VFile *file = (VFile *)lmalloc(sizeof(VFile));

        bool found = false;

        while (fs->fsFunctionTable->Next(fs, iter, file) == VFS_PASSED) {
            if (! strcmp(file->filename, filename)) {
                found = true;
                break;
            }
            fs->fsFunctionTable->Close(file);
        }

        fs->fsFunctionTable->CloseIteration(fs, iter);

        // 未找到
        if (! found) {
            fs->fsFunctionTable->Close(file);
            lfree(file);
            LogError("未找到文件%s(%s文件/目录未找到)", backupPathPtr, filename);
            return NULL;
        }

        if (! dir->isRoot) {
            fs->fsFunctionTable->Close(dir);
            lfree(dir);
        }

        dir = file;
    }

    return dir;
}

/**
 * @brief 关闭文件
 *
 * @param file 文件
 */
void CloseFile(VFile *file) {
    file->fsPtr->fsFunctionTable->Close(file);
    lfree(file);
}