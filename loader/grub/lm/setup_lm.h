/**
 * @file setup_lm.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 进入长模式
 * @version alpha-1.0.0
 * @date 2023-08-17
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <tay/types.h>

/**
 * @brief 页表区域起始地址
 *
 */
#define PAGING_TABLE_ADDRESS (0x1800000)

/**
 * @brief 分页范围
 *
 */
#define PAGING_RANGE         (0x2000000)

/**
 * @brief 配置分页
 *
 */
void SetupPaging(void);