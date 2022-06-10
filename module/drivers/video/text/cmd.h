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
 * cmd.h
 *
 * 命令处理器
 *
 */



#pragma once

#include <tayhuang/types.h>
#include <modes.h>

void deal_text_cmd(int caller, int cmd, qword *param);

#define __TEXT_WRITE_CHAR (0)
#define TEXT_WRITE_CHAR MKCMD(MODE_TEXT, __TEXT_WRITE_CHAR)

#define __TEXT_WRITE_CHARS (1)
#define TEXT_WRITE_CHARS MKCMD(MODE_TEXT, __TEXT_WRITE_CHARS)