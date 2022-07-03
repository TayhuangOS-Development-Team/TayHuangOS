/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * partition.h
 *
 * 分区相关
 *
 */



#pragma once

#include <tayhuang/defs.h>

#define PARTITION_TABLE_OFFSET (0x01BE)
#define PARTITION_NUMBER (4)

//分区状态
enum {
    PS_BOOTABLE = 0x80,
    PS_UNBOOTABLE = 0x00
};

//分区成员
struct __partition_member {
    byte state;
    byte start_head;
    byte start_sector;
    byte start_cylinder;
    byte system_id;
    byte end_head;
    byte end_cylinder;
    byte end_sector;
    dword start_lba;
    dword sector_number;
} __attribute__((packed));

typedef struct __partition_member partition_member; 