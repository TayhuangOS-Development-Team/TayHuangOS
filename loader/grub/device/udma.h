/**
 * @file udma.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief UDMA
 * @version alpha-1.0.0
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tay/types.h>

/**
 * @brief 初始化DMA
 * 
 */
void init_dma(void);

/**
 * @brief 重置prdt
 * 
 */
void reset_prdt(void);

/**
 * @brief 添加传输项
 * 
 * @param addr 地址
 * @param cnt 字节数
 * @param lba LBA
 * @param read 是否读取
 */
void append_transfer(void *addr, word cnt, dword lba, bool read);

/**
 * @brief 开始传输
 * 
 */
void start_transfer(void);