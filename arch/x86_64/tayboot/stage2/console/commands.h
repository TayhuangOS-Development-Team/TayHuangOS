// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/console/commands.h
 *
 * Real mode console commands are declared here
 */



#pragma once

#include "../header.h"

#define CMD_NAME(x) __console_cmd_##x##__
#define DEF_CONSOLE_CMD(x) PUBLIC int CMD_NAME(x)(int argc, const char** argv)

PUBLIC void init_variables(void);
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
extern PUBLIC bool logined;