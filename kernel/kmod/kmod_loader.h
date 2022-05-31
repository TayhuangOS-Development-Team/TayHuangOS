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
 * kmod_loader.h
 *
 * 内核模块加载器
 *
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct {
    void *start;
    void *limit;
    void *entry;
    void *pgd;
    void *stack_top;
    void *stack_bottom;
} program_info;

PUBLIC program_info load_kmod_from_memory(void *addr);