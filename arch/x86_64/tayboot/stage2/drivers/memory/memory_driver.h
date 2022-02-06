/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/memory/memory_driver.h
 *
 * 内存驱动
 */



#pragma once

#include "../driver.h"

#define MM_CMD_INIT_BUFFER CMD_TY(1) // NULL
#define MM_CMD_RESET_BUFFER CMD_TY(2) // NULL
#define MM_CMD_ALLOC CMD_TY(3) // PAPACK(mm, alloc)
#define MM_CMD_FREE CMD_TY(4) // addr_t*
#define MM_CMD_GET_USED CMD_TY(5) // dword*
#define MM_CMD_GET_REMAIN CMD_TY(6) // dword*
#define MM_CMD_GET_DATA CMD_TY(7) // PAPACK(mm, get_data)
#define MM_CMD_SET_DATA CMD_TY(8) // PAPACK(mm, set_data)
#define MM_CMD_CP_DATA CMD_TY(9) // PAPACK(mm, cp_data)
#define MM_CMD_PRODE CMD_TY(10) //pmem_prode
#define MM_CMD_GET_MEM_SIZE CMD_TY(15) // dword*
#define MM_CMD_UPDATE CMD_TY(16)  // NULL
#define MM_CMD_REFRESH_GC CMD_TY(17) // addr_t*
#define MM_CMD_GET_BUFFER_SEGMENT CMD_TY(18) //sreg_t*

#define ARDS_MEM_MEMORY 1
#define ARDS_MEM_RESERVED 2

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

typedef struct {
    bool weak;
    dword length;
    addr_t* address;
} DAPACK(mm, alloc_buffer);

typedef struct {
    addr_t src;
    void *dst;
    dword size;
} DAPACK(mm, get_data);

typedef struct {
    void *src;
    addr_t dst;
    dword size;
} DAPACK(mm, set_data);

typedef struct {
    addr_t src;
    addr_t dst;
    dword size;
} DAPACK(mm, cp_data);

PUBLIC void create_memory_driver(pdriver driver);
PUBLIC dword get_memsz_high(void);