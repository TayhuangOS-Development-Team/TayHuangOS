/**
 * @file elfLoader.h
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
    void *entrypoint;

    /** 起始地址 */
    void *start;
    /** 结束地址 */
    void *limit;
} LoadInfo;

/**
 * @brief 加载ELF文件
 *
 * @param addr ELF文件地址
 * @param info 加载信息
 * @return 加载是否成功
 */
bool LoadELF(void *addr, LoadInfo *info);