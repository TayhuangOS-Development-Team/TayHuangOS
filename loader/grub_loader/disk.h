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
 * disk.h
 *
 * 硬盘操作函数
 *
 */



#pragma once

#include <tayhuang/descs.h>
#include <tayhuang/partition.h>

#define DISK_SEL_IDE0 (0)
#define DISK_SEL_IDE1 (1)
#define DISK_SEL_MASTER (0)
#define DISK_SEL_SLAVE (2)
#define DISK_SEL_IDE0_MASTER (DISK_SEL_IDE0 | DISK_SEL_MASTER)
#define DISK_SEL_IDE0_SLAVE (DISK_SEL_IDE0 | DISK_SEL_SLAVE)
#define DISK_SEL_IDE1_MASTER (DISK_SEL_IDE1 | DISK_SEL_MASTER)
#define DISK_SEL_IDE1_SLAVE (DISK_SEL_IDE1 | DISK_SEL_SLAVE)

PUBLIC void init_disk_driver(void);
PUBLIC void identify_disk(int selector, void *dst); //获取硬盘参数
PUBLIC void read_sector(dword lba, int num, int selector, void *dst); //读扇区
PUBLIC void get_partition(int selector, int number, partition_member *member);