/**
 * @file main.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief Loader主函数
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <tay/types.h>
#include <tay/ports.h>

#include <init/gdt.h>
#include <init/idt.h>

#include <stdbool.h>

#include <libs/capi.h>
#include <libs/debug.h>

#include <primlib/logger.h>

#include <device/disk.h>

/**
 * @brief 初始化
 * 
 */
void initialize(void) {
    init_gdt();

    init_serial();
    init_ulogger(write_serial_str, "GRUB Loader");

    init_pic();
    init_idt();
}

/**
 * @brief 结束
 * 
 */
void terminate(void) {
}

/**
 * @brief 入口函数
 * 
 */
void main(void) {
    initialize();

    asm volatile ("sti");
    
    disk_t *disk = load_disk(IDE0_BASE, IDE0_BASE2, false);
    log_info("%s", disk->serial);
    log_info("%s", disk->model);

    while (true);
    
    terminate();
}