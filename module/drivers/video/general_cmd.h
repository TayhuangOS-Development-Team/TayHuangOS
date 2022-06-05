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
 * general_cmd.h
 *
 * 通用命令
 *
 */



#pragma once

#include <modes.h>

#define __CLEAR_SCREEN (0)
#define CLEAR_SCREEN MKCMD(MODE_ANY, __CLEAR_SCREEN)