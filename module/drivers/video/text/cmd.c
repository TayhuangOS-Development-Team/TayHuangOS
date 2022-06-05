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
 * cmd.c
 *
 * 命令处理器
 *
 */



#include "cmd.h"

#include <debug/logging.h>

void deal_text_cmd(int caller, int cmd, qword *param) {
    switch (cmd)
    {
    case __TEXT_WRITE_CHAR: {
        char ch = param[0];
        int x = param[1];
        int y = param[2];
        *(char*)(0xB8000 + (y * 80 + x) * 2) = ch;
        *(char*)(0xB8001 + (y * 80 + x) * 2) = 0x0F;
        break;
    }
    default:
        break;
    }
}