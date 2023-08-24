/**
 * @file elf_loader.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 内核加载
 * @version alpha-1.0.0
 * @date 2023-08-16
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <stdbool.h>

/**
 * @brief 加载信息
 *
 */
typedef struct {
    /** 入口点 */
    void *programEntrypoint;

    /** 起始地址 */
    void *programStartAddress;
    /** 结束地址 */
    void *programLimitAddress;
} LoadInfo;

/**
 * @brief 加载ELF文件
 *
 * @param elfAddress ELF文件地址
 * @param info 加载信息
 * @return 加载是否成功
 */
bool LoadELF(void *elfAddress, LoadInfo *info);