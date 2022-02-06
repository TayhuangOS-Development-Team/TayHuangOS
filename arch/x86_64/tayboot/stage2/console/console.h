/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/console/console.h
 *
 * 实模式控制台
 * 没什么实际作用
 */



#pragma once

#include "../header.h"

PUBLIC void enter_console(void);
extern PUBLIC char user_name[32]; //用户名
extern PUBLIC bool logined; //是否登陆