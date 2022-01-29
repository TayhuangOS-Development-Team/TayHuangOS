// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/disk/filesystem.h
 *
 * File system functions are declared here
 */



#pragma once

#include "../driver.h"

#define FILESYSTEM_EXT_MAGIC (0x8C4BAD95)

PUBLIC void* recognize_file_system(pdriver disk_driver);