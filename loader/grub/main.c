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

#include <init/gdt.h>

#include <stdbool.h>

#include <libs/capi.h>
#include <libs/debug.h>

#include <primlib/logger.h>

/**
 * @brief 入口函数
 * 
 */
void main(void) IMPL("C") {
    init_gdt();

    init_serial();
    init_ulogger(write_serial_str, "GRUB Loader");
    while (true);
}