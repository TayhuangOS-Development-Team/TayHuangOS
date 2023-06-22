/**
 * @file udma.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief UDMA
 * @version alpha-1.0.0
 * @date 2023-06-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <device/udma.h>
#include <string.h>

#define PRDT_TABLE_SIZE (64)

// PRD项
typedef struct {
    dword addr;
    word count;
    word msb;
} prd_t;

// PRD附加信息
typedef struct {
    dword lba;
    bool read;
} prd_ext_t;

// PRD表
static prd_t PRDT[PRDT_TABLE_SIZE];
// PRD LBA表
static prd_ext_t PRD_EXT[PRDT_TABLE_SIZE];
// 队列指针
static dword trans_ptr;

/**
 * @brief 初始化DMA
 * 
 */
void init_dma(void) {
    // 设置BMR

    // 重置PRDT
    reset_prdt();
}

/**
 * @brief 重置prdt
 * 
 */
void reset_prdt(void) {
    memset(PRDT, 0, sizeof(PRDT));
    memset(PRD_EXT, 0, sizeof(PRD_EXT));

    // 重置指针
    trans_ptr = 0;
}

static bool check_queue(void) {
    return trans_ptr < PRDT_TABLE_SIZE;
}

/**
 * @brief 添加传输项
 * 
 * @param addr 地址
 * @param cnt 字节数
 * @param lba LBA
 * @param read 是否读取
 */
void append_transfer(void *addr, word cnt, dword lba, bool read) {
    if (! check_queue()) {
        return;
    }

    PRDT[trans_ptr].addr = (dword)(addr);
    PRDT[trans_ptr].count = cnt;

    PRD_EXT[trans_ptr].lba = lba;
    PRD_EXT[trans_ptr].read = read;

    trans_ptr ++;    
}

/**
 * @brief 开始传输
 * 
 */
void start_transfer(void) {
    // 无传输项
    if (trans_ptr <= 0) {
        return;
    }

    // 标记最后一项
    PRDT[trans_ptr - 1].msb = 1;
    
    // 开始传输

    //...

    // 重置PRDT
    reset_prdt();
}