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
 * arch/x86_64/tayboot/stage2/console/commands.h
 *
 * 实模式命令
 */



#pragma once

#include "../header.h"

//命名宏
#define CMD_NAME(x) __console_cmd_##x##__
#define DEF_CONSOLE_CMD(x) PUBLIC int CMD_NAME(x)(int argc, const char** argv)

//初始化变量表
PUBLIC void init_variables(void);
//命令列表
DEF_CONSOLE_CMD(echo);
DEF_CONSOLE_CMD(echoln);
DEF_CONSOLE_CMD(shutdown);
DEF_CONSOLE_CMD(help);
DEF_CONSOLE_CMD(time);
DEF_CONSOLE_CMD(random);
DEF_CONSOLE_CMD(change_name);
DEF_CONSOLE_CMD(reboot);
DEF_CONSOLE_CMD(guess_number);
DEF_CONSOLE_CMD(cls);
DEF_CONSOLE_CMD(set_pwd);
DEF_CONSOLE_CMD(login);
DEF_CONSOLE_CMD(ls);
DEF_CONSOLE_CMD(set);
DEF_CONSOLE_CMD(goto_os);
extern PUBLIC bool logined; //是否登陆