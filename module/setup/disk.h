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
 * disk.h
 *
 * 硬盘操作
 *
 */



#pragma once

#include <types.h>

void load_module(const char *name, void *addr);
void identify_ide0_disk(bool slave, void *dst);