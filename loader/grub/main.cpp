/**
 * @file main.cpp
 * @author theflysong (song_of_the_fly@163.com)
 * @brief GRUB 2 Loader 入口
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <tayhuang/attributes.h>
#include <tayhuang/types.h>

/**
 * @brief GRUB 2 Loader 入口点
 * 
 */
EXPORT IMPL NORETURN void entry(void) {
    *(short*)(0xB8000) = 0x0C40;
    *(short*)(0xB8002) = 0x0C41;
    *(short*)(0xB8004) = 0x0C42;
    *(short*)(0xB8006) = 0x0C43;
    int *p = new int;
    *p = 0x20;
    int *q = new int;
    *q = 0x25;
    *(short*)(0xB8008) = 0x0C00 + ((*p + *q) & 0xFF);
    while (true);
}