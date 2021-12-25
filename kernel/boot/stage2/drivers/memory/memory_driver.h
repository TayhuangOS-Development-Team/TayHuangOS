// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/memory/memory_driver.h
 *
 * Memory driver is declared here
 */



#pragma once

#include "../driver.h"

#define MM_CMD_INIT CMD_TY(0)
#define MM_CMD_INIT_HEAP CMD_TY(1)
#define MM_CMD_RESET_HEAP CMD_TY(2)
#define MM_CMD_ALLOC CMD_TY(3)
#define MM_CMD_FREE CMD_TY(4)
#define MM_CMD_GET_USED CMD_TY(5)
#define MM_CMD_GET_REMAIN CMD_TY(6)
#define MM_CMD_GET_DATA CMD_TY(7)
#define MM_CMD_SET_DATA CMD_TY(8)
#define MM_CMD_CP_DATA CMD_TY(9)
#define MM_CMD_RESIZE CMD_TY(10)
#define MM_CMD_PRODE CMD_TY(11)
#define MM_CMD_TEST_A20 CMD_TY(12)
#define MM_CMD_FAST_TEST_A20 CMD_TY(13)
#define MM_CMD_OPEN_A20 CMD_TY(14)
#define MM_CMD_CLOSE_A20 CMD_TY(15)

typedef struct {
    dword base_address_low;
    dword base_address_high;
    dword length_low;
    dword length_high;
    dword type;
} ards_t, pards;

typedef struct {
    ards_t ards_list[20];
    int prode_cnt;
} mem_prode_t, *pmem_prode;


PUBLIC void create_memory_driver(pdriver driver);