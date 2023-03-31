/**
 * @file main.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief Main所在文件
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <tay/types.h>

#include <stdbool.h>

/**
 * @brief 入口函数
 * 
 */
void main(void) IMPL("C") {
    *(short*)(0xB8000) = 0x0C40;
    *(short*)(0xB8002) = 0x0C41;
    *(short*)(0xB8004) = 0x0C42;
    *(short*)(0xB8006) = 0x0C43;
    while (true);
}