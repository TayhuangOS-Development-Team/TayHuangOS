/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * load_kernel.h
 * 
 * 加载内核
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define KERNEL_BIN_SIZE (256 * 1024)
#define SETUP_MOD_SIZE (256 * 1024)

typedef struct {
    bool status;
    void *kernel_start;
    void *kernel_limit;
    void *kernel_entry;
    void *setup_mod;
} load_result_struct;

PUBLIC void load_kernel(load_result_struct *result);