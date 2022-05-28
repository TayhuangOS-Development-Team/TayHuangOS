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

PUBLIC void load_kmod_from_memory(void *addr);