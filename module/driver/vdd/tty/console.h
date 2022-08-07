/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * console.h
 * 
 * 控制台
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/paging.h>
#include <export/keyboard/__keyboard_driver_fn.h>

//控制台结构
typedef struct {
    void *framebuffer;
    int framebuffer_id;
    int console_no;
    int pos_x;
    int pos_y;
    int color;
} console_t;

//相关信息
#define CONSOLE_NUM (4)
#define CONSOLE_COLUMNS (80)
#define CONSOLE_LINES (25)
#define CONSOLE_SIZE (CONSOLE_LINES * CONSOLE_COLUMNS * 2)
#define CONSOLE_PAGES ((CONSOLE_SIZE + (MEMUNIT_SZ - 1)) / MEMUNIT_SZ)

EXTERN PUBLIC console_t consoles[CONSOLE_NUM];
EXTERN PUBLIC int current_active_console;
EXTERN PUBLIC id_t wait_enter;

PUBLIC void init_consoles(void);
PUBLIC void console_register_rpc_functions(void);
//回显
PUBLIC void echo_key(key_t key);