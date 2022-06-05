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

void deal_graphic_cmd(int caller, int cmd, qword *param);
void dispatch_txt_cmd(int caller, int cmd, qword *param);