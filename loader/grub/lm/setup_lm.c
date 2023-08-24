/**
 * @file setup_lm.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 进入长模式
 * @version alpha-1.0.0
 * @date 2023-08-17
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <lm/setup_lm.h>
#include <tay/paging.h>

/**
 * @brief 配置分页
 *
 */
void SetupPaging(void) {
    // 1:1映射
    qword mappingPages = PAGING_RANGE / PAGE_SIZE;
}