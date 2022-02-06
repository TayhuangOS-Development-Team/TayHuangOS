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
 * disk_driver.h
 *
 * 硬盘驱动
 *
 */



#pragma once

#include "../driver.h"
#include "../../file.h"

#define DK_CMD_READ_SECTOR CMD_TY(0)
#define DK_CMD_INIT CMD_TY(1)
#define DK_CMD_GET_FILESYSTEM CMD_TY(2)
#define DK_CMD_LOAD_FILE CMD_TY(3)
#define DK_CMD_LOAD_FILE_INFO CMD_TY(4)
#define DK_CMD_SAVE_FILE CMD_TY(5)
#define DK_CMD_SAVE_FILE_INFO CMD_TY(6)
#define DK_CMD_SAVE_SECTOR CMD_TY(7)
#define DK_CMD_CREATE_FILE CMD_TY(8)
#define DK_CMD_FOREACH_FILE CMD_TY(9)

typedef struct {
    addr_t dst;
    dword src_high;
    dword src_low;
} DAPACK(dk, read_sector);

typedef struct {
    addr_t src;
    dword dst_high;
    dword dst_low;
} DAPACK(dk, save_sector);


typedef struct {
    sreg_t segment;
    addr_t offset;
    const char *name;
} DAPACK(dk, load_file);

typedef struct {
    const char *name;
    file_info_t *info;
} DAPACK(dk, load_file_info);

typedef struct {
    sreg_t segment;
    addr_t offset;
    const char *name;
} DAPACK(dk, save_file);

typedef struct {
    const char *name;
    file_info_t *info;
} DAPACK(dk, save_file_info);

typedef struct {
    const char *name;
    file_info_t *output;
} DAPACK(dk, create_file);

typedef struct {
    char *output;
    int next;
} DAPACK(dk, foreach_file);

PUBLIC void create_disk_driver(pdriver driver);
PUBLIC void __read_sector(pdriver driver, sreg_t segment, addr_t offset, dword lba_high, dword lba_low);
PUBLIC void read_sector(pdriver driver, addr_t buffer, dword lba_high, dword lba_low);
PUBLIC void __save_sector(pdriver driver, sreg_t segment, addr_t offset, dword lba_high, dword lba_low);
PUBLIC void save_sector(pdriver driver, addr_t buffer, dword lba_high, dword lba_low);